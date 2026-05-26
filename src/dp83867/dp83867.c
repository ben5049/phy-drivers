/*
 * dp83867.c
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#include "phy_utils.h"
#include "phy_io.h"

#include "dp83867.h"
#include "dp83867_init.h"
#include "dp83867_regs.h"


phy_status_t PHY_DP83867_GetLinkState(phy_handle_dp83867_t *dev, bool *linkup) {

    phy_status_t status          = PHY_OK;
    uint16_t     reg_data        = 0;
    bool         linkup_internal = false;

    PHY_LOCK;

    /* Read the basic mode status register */
    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_BMSR, &reg_data);
    PHY_CHECK_END(status);

    /* Extract the link status bit */
    linkup_internal = (reg_data & PHY_DP83867_LINK_STATUS) != 0;

    /* If there is a change then call the corresponding callback */
    if (dev->linkup != linkup_internal) {
        status = dev->callbacks->callback_event(linkup_internal ? PHY_EVENT_LINK_UP : PHY_EVENT_LINK_DOWN, dev->callback_context);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    dev->linkup = linkup_internal;

    /* Update the output */
    if (linkup != NULL) *linkup = dev->linkup;

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_SetSpeed(phy_handle_dp83867_t *dev, phy_speed_t speed) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    PHY_LOCK;

    // TODO: Implement

    // end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_GetSpeed(phy_handle_dp83867_t *dev, phy_speed_t *speed) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the PHY status register */
    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_PHYSTS, &reg_data);
    PHY_CHECK_END(status);

    /* Decode the speed */
    switch ((reg_data & PHY_DP83867_SPEED_MASK) >> PHY_DP83867_SPEED_SHIFT) {

        case PHY_DP83867_SPEED_10MBPS:
            dev->speed = PHY_SPEED_10M;
            break;

        case PHY_DP83867_SPEED_100MBPS:
            dev->speed = PHY_SPEED_100M;
            break;

        case PHY_DP83867_SPEED_1000MBPS:
            dev->speed = PHY_SPEED_1G;
            break;

        default:
            status = PHY_INVALID_REGISTER_CONTENT_ERROR;
            break;
    }
    PHY_CHECK_END(status);

    /* Assign the output */
    if (speed != NULL) *speed = dev->speed;

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_GetDuplex(phy_handle_dp83867_t *dev, phy_duplex_t *duplex) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the PHY status register */
    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_PHYSTS, &reg_data);
    PHY_CHECK_END(status);

    if (reg_data & PHY_DP83867_PHYSTS_DUPLEX) {
        dev->duplex = PHY_FULL_DUPLEX;
    } else {
        dev->duplex = PHY_HALF_DUPLEX;
    }

    /* Assign the output */
    if (duplex != NULL) *duplex = dev->duplex;

end:

    PHY_UNLOCK;
    return status;
}


/* Converts DP83867 MSE to a 0-100 Signal Quality Indicator (SQI)
 *
 * Maps:
 * SNR < 19 (Poor)      -> SQI < 40
 * SNR 19-21 (Good)     -> SQI 40-60
 * SNR > 21 (Excellent) -> SQI > 60
 */
static inline uint8_t mse_to_sqi(uint16_t mse) {

    /* Prevent log2(0) */
    if (mse == 0) {
        return 100;
    }

    uint32_t int_log2;
    uint32_t frac;
    uint32_t log2_q8;
    int32_t  snr_q8;
    int32_t  sqi;

    /* Fast log2 approximation in Q8 format */
    int_log2 = 31 - __builtin_clz(mse);

    /* Linear interpolation for the fractional part: (mse - 2^int_log2) / 2^int_log2 */
    frac    = ((mse - (1 << int_log2)) << 8) >> int_log2;
    log2_q8 = (int_log2 << 8) + frac;

    /* Calculate SNR in Q8 format
     * Equation: SNR = 48.175 - 3.0103 * log2(MSE)
     * In Q8 (multiply constants by 256):
     * 48.175 * 256 = 12333
     * 3.0103 * 256 = 771 */
    snr_q8 = 12333 - ((771 * log2_q8) >> 8);

    /* Map SNR to 0-100 scale (SQI)
     * Formula: SQI = (SNR - 15) * 10
     * In Q8: SQI = (SNR_Q8 - (15 * 256)) * 10 / 256 */
    sqi = ((snr_q8 - 3840) * 10) >> 8;

    /* Clamp and return the output */
    return (uint8_t) CONSTRAIN(sqi, 0, 100);
}

/* Get the signal quality indicator from the mean square error rate */
phy_status_t PHY_DP83867_GetSQI(phy_handle_dp83867_t *dev, uint8_t *sqi) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;
    uint8_t      sqi_internal;

    PHY_LOCK;

    /* Not supported for 10mbps */
    if (dev->speed == PHY_SPEED_10M) {
        sqi_internal = dev->linkup ? 100 : 0;
    }

    /* 100mbps uses only channel A */
    else if (dev->speed == PHY_SPEED_100M) {
        status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_EXT_MSE_CHANNEL_A, &reg_data);
        PHY_CHECK_END(status);
        sqi_internal = mse_to_sqi(reg_data);
    }

    /* 1000mbps uses all 4 channels */
    else if (dev->speed == PHY_SPEED_1G) {

        uint8_t sqi_channel;

        /* Channel A */
        status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_EXT_MSE_CHANNEL_A, &reg_data);
        PHY_CHECK_END(status);
        sqi_internal = mse_to_sqi(reg_data);

        /* Channel B */
        status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_EXT_MSE_CHANNEL_B, &reg_data);
        PHY_CHECK_END(status);
        sqi_channel  = mse_to_sqi(reg_data);
        sqi_internal = MIN(sqi_internal, sqi_channel);

        /* Channel C */
        status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_EXT_MSE_CHANNEL_C, &reg_data);
        PHY_CHECK_END(status);
        sqi_channel  = mse_to_sqi(reg_data);
        sqi_internal = MIN(sqi_internal, sqi_channel);

        /* Channel D */
        status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_EXT_MSE_CHANNEL_D, &reg_data);
        PHY_CHECK_END(status);
        sqi_channel  = mse_to_sqi(reg_data);
        sqi_internal = MIN(sqi_internal, sqi_channel);
    }

    /* Invalid speed */
    else {
        status = PHY_INVALID_SPEED_ERROR;
        PHY_CHECK_END(status);
    }

    *sqi = sqi_internal;

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_EnableIEEEPowerDown(phy_handle_dp83867_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the basic mode control register */
    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_BMCR, &reg_data);
    PHY_CHECK_END(status);

    /* If not powered down the set the low power bit */
    if (!(reg_data & PHY_DP83867_POWER_DOWN)) {
        reg_data |= PHY_DP83867_POWER_DOWN;
        status    = phy_dp83867_write_reg(dev, PHY_DP83867_REG_BASIC_BMCR, reg_data);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    dev->linkup = false;

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_DisableIEEEPowerDown(phy_handle_dp83867_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the basic mode control register */
    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_BMCR, &reg_data);
    PHY_CHECK_END(status);

    /* If powered down the clear the low power bit */
    if (reg_data & PHY_DP83867_POWER_DOWN) {
        reg_data &= ~PHY_DP83867_POWER_DOWN;
        status    = phy_dp83867_write_reg(dev, PHY_DP83867_REG_BASIC_BMCR, reg_data);
        PHY_CHECK_END(status);
    }

    status = phy_dp83867_read_reg(dev, PHY_DP83867_REG_BASIC_BMCR, &reg_data);
    PHY_CHECK_END(status);

end:

    PHY_UNLOCK;
    return status;
}
