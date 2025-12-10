/*
 * 88q211x_bist.c
 *
 *  Created on: Aug 14, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/88q211x/88q211x_bist.h"
#include "internal/88q211x/88q211x_regs.h"
#include "internal/phy_utils.h"


/* TODO: Take configuration options */
phy_status_t PHY_88Q211X_EnablePacketGenerator(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    /* Get the current packet generator state */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Enable the packet generator */
    reg_data |= 0xff00; /* Set the burst size to 255 */
    reg_data |= PHY_88Q211X_PACKET_GEN_EN;
    reg_data |= PHY_88Q211X_PACKET_GEN_TRANSMIT;

    /* Write the new current packet generator state */
    status = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL, reg_data);
    PHY_CHECK_RET(status);

    return status;
}


phy_status_t PHY_88Q211X_DisablePacketGenerator(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    /* Get the current packet generator state */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Only disable if the packet generator was enabled */
    if (reg_data & PHY_88Q211X_PACKET_GEN_EN) {
        reg_data &= ~PHY_88Q211X_PACKET_GEN_EN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL, reg_data);
        PHY_CHECK_RET(status);
    }

    return status;
}


phy_status_t PHY_88Q211X_EnablePacketChecker(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    /* Get the current packet checker state */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Return early if the packet checker is already enabled */
    if (reg_data & PHY_88Q211X_PACKET_CHECK_EN) return status;

    /* Step 1: Start the counter */
    reg_data |= PHY_88Q211X_PACKET_CHECK_SAMPLE;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, reg_data);
    PHY_CHECK_RET(status);

    /* Step 2: Start the packet checker */
    reg_data |= PHY_88Q211X_PACKET_CHECK_SAMPLE;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, reg_data);
    PHY_CHECK_RET(status);

    return status;
}


phy_status_t PHY_88Q211X_DisablePacketChecker(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;
    uint8_t      errors;

    /* Get the packet and error counts and reset the packet checker */
    status = PHY_88Q211X_ReadPacketCheckerCounters(dev, NULL, &errors, true);
    PHY_CHECK_RET(status);
    dev->events.crc_errors += errors;

    /* Get the current packet checker state */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Only disable if the packet checker was enabled (this will also clear the counters) */
    if (reg_data & PHY_88Q211X_PACKET_CHECK_EN) {
        reg_data &= ~PHY_88Q211X_PACKET_CHECK_EN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, reg_data);
        PHY_CHECK_RET(status);
    }

    return status;
}


phy_status_t PHY_88Q211X_ReadPacketCheckerCounters(phy_handle_88q211x_t *dev, uint8_t *packets_received, uint8_t *errors, bool clear) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    /* Get the packet checker counters */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_COUNT, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_COUNT, &reg_data);
    PHY_CHECK_RET(status);

    /* Extract the data */
    if (packets_received != NULL) *packets_received = (reg_data & PHY_88Q211X_PACKET_CHECK_COUNT_MASK) >> PHY_88Q211X_PACKET_CHECK_COUNT_SHIFT;
    if (errors != NULL) *errors = (reg_data & PHY_88Q211X_PACKET_CHECK_ERRORS_MASK) >> PHY_88Q211X_PACKET_CHECK_ERRORS_SHIFT;

    /* Clear the counters if required */
    if (clear) {

        /* Get the current packet checker status */
        status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, &reg_data);
        PHY_CHECK_RET(status);

        /* Reset the counters */
        reg_data &= ~PHY_88Q211X_PACKET_CHECK_COUNTER_RST;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL, reg_data);
        PHY_CHECK_RET(status);
    }

    return status;
}


phy_status_t PHY_88Q211X_ConfigureGMIISteering(phy_handle_88q211x_t *dev, phy_host_rx_88q211x_t host_rx, phy_line_tx_88q211x_t line_tx, phy_packet_check_88q211x_t packet_check) {

    phy_status_t status = PHY_OK;
    uint16_t     old_pcs_config;
    uint16_t     new_pcs_config;

    /* Disable the packet checker and generator */
    status = PHY_88Q211X_DisablePacketChecker(dev);
    PHY_CHECK_RET(status);
    status = PHY_88Q211X_DisablePacketGenerator(dev);
    PHY_CHECK_RET(status);

    /* Get the current PCS configuration */
    status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PCS_CTRL, PHY_88Q211X_REG_1000BASE_T1_PCS_CTRL, &old_pcs_config);
    PHY_CHECK_RET(status);

    /* Update the PCS configuration */
    new_pcs_config  = old_pcs_config;
    new_pcs_config |= ((uint16_t) host_rx << PHY_88Q211X_GMII_STEERING_MUX_A_SHIFT) & PHY_88Q211X_GMII_STEERING_MUX_A_MASK;
    new_pcs_config |= ((uint16_t) line_tx << PHY_88Q211X_GMII_STEERING_MUX_B_SHIFT) & PHY_88Q211X_GMII_STEERING_MUX_B_MASK;
    new_pcs_config |= ((uint16_t) packet_check << PHY_88Q211X_GMII_STEERING_MUX_C_SHIFT) & PHY_88Q211X_GMII_STEERING_MUX_C_MASK;

    /* Only write the new PCS config is it has changed */
    if (new_pcs_config != old_pcs_config) {
        status = PHY_WRITE_REG(PHY_88Q211X_DEV_1000BASE_T1_PCS_CTRL, PHY_88Q211X_REG_1000BASE_T1_PCS_CTRL, new_pcs_config);
        PHY_CHECK_RET(status);
    }

    /* Re-enable the packet checker */
    status = PHY_88Q211X_EnablePacketChecker(dev);
    PHY_CHECK_RET(status);

    return status;
}


phy_status_t PHY_88Q211X_ResetGMIISteering(phy_handle_88q211x_t *dev) {
    return PHY_88Q211X_ConfigureGMIISteering(dev, PHY_HOST_RX_88Q211X_LINE_RX, PHY_LINE_TX_88Q211X_HOST_TX, PHY_PACKET_CHECK_88Q211X_LINE);
}


phy_status_t PHY_88Q211X_Start100MBIST(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    /* Set the output of the packet generator to be the input of the packet checker */
    status = PHY_88Q211X_ConfigureGMIISteering(dev, PHY_HOST_RX_88Q211X_LINE_RX, PHY_LINE_TX_88Q211X_HOST_TX, PHY_PACKET_CHECK_88Q211X_PACKET_GEN);
    PHY_CHECK_END(status);

    /* Enable the packet checker */
    status = PHY_88Q211X_EnablePacketChecker(dev);
    PHY_CHECK_END(status);

    /* Enable the packet generator */
    status = PHY_88Q211X_EnablePacketGenerator(dev);
    PHY_CHECK_END(status);

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_Stop100MBIST(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    /* Set the steering back to normal (also disables the packet generator) */
    status = PHY_88Q211X_ResetGMIISteering(dev);
    PHY_CHECK_END(status);

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_Get100MBISTResults(phy_handle_88q211x_t *dev, bool *error) {

    phy_status_t status           = PHY_OK;
    uint16_t     reg_data         = 0;
    uint8_t      packets_received = 0;
    uint8_t      errors           = 0;
    bool         done             = false;

    PHY_LOCK;

    /* Poll until data is ready */
    for (uint_fast8_t i = 0; !done && (i < 10); i++) {

        /* Get the current packet generator state */
        status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL, PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL, &reg_data);
        PHY_CHECK_END(status);

        /* Check if all packets have been generated (enable self clears) */
        if (!(reg_data & PHY_88Q211X_PACKET_GEN_EN)) {
            done = true;
        } else {
            dev->callbacks->callback_delay_ms(dev->config.timeout / 10, dev->callback_context);
        }
    }

    /* Timed out */
    if (!done) {
        status = PHY_88Q211X_Stop100MBIST(dev);
        PHY_CHECK_END(status);
        status = PHY_TIMEOUT;
        goto end;
    }

    /* Get the results */
    status = PHY_88Q211X_ReadPacketCheckerCounters(dev, &packets_received, &errors, false);
    PHY_CHECK_END(status);

    /* If there were any errors then set error to true */
    *error = (bool) errors;

    /* TODO: Calculate the error percentage */

    /* Stop the BIST */
    status = PHY_88Q211X_Stop100MBIST(dev);
    PHY_CHECK_END(status);

end:

    PHY_UNLOCK;
    return status;
}


/* This can take up to 500ms */
phy_status_t PHY_88Q211X_StartVCT(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    uint16_t reg_data;

    /* Step 1: Ignore wire activity (register 3.FEC9 bit 7 to 1). Note this register isn't described in the datasheet's registers section! */
    status = PHY_READ_REG(0x03, 0xfec9, &reg_data);
    PHY_CHECK_END(status);
    reg_data |= 1 << 7;
    status    = PHY_WRITE_REG(0x03, 0xfec9, reg_data);
    PHY_CHECK_END(status);

    /* Step 2: Fix incoming ADC sign bit (register 3.FEC3 bit 13 to 1). Note that this bit isn't described in the datasheet's registers section! */
    status = PHY_READ_REG(PHY_88Q211X_DEV_TDR_CTRL, PHY_88Q211X_REG_TDR_CTRL, &reg_data);
    PHY_CHECK_END(status);
    reg_data |= 1 << 13;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_TDR_CTRL, PHY_88Q211X_REG_TDR_CTRL, reg_data);
    PHY_CHECK_END(status);

    /* Step 3: Adjust threshold (register 3.FEC4 = 0x0F20). Note this register isn't described in the datasheet's registers section! */
    reg_data = 0x0f20;
    status   = PHY_WRITE_REG(0x03, 0xfec4, reg_data);
    PHY_CHECK_END(status);

    /* Step 4: Adjust threshold (register 3.FEC7 = 0x1219). Note this register isn't described in the datasheet's registers section! */
    reg_data = 0x1219;
    status   = PHY_WRITE_REG(0x03, 0xfec7, reg_data);
    PHY_CHECK_END(status);

    /* Step 5: Enable TDR function (register 3.FEC3 bit 14 to 1, self-clearing) */
    status = PHY_READ_REG(PHY_88Q211X_DEV_TDR_CTRL, PHY_88Q211X_REG_TDR_CTRL, &reg_data);
    PHY_CHECK_END(status);
    reg_data |= PHY_88Q211X_TDR_EN;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_TDR_CTRL, PHY_88Q211X_REG_TDR_CTRL, reg_data);
    PHY_CHECK_END(status);

    /* TODO: Check if bit 12 needs to be set? "Start TDR Test" bit */
end:

    PHY_UNLOCK;
    return status;
}


/* Get the virtual cable test result, and maximum peak distance in mm */
phy_status_t PHY_88Q211X_GetVCTResults(phy_handle_88q211x_t *dev, phy_cable_state_88q211x_t *cable_state, uint32_t *maximum_peak_distance) {

    phy_status_t status = PHY_OK;
    PHY_LOCK;

    uint16_t reg_data;
    bool     test_complete = false;
    uint16_t vct_distance;
    bool     vct_polarity;
    uint8_t  vct_amplitude;

    /* Read register 3.FEDB[10] set to 1 when VCT data is ready */
    for (uint_fast8_t i = 0; i < 10; i++) {

        /* Read the status register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_TDR_STATUS_1, PHY_88Q211X_REG_TDR_STATUS_1, &reg_data);
        PHY_CHECK_END(status);

        /* Check if the test is complete */
        if (reg_data & PHY_88Q211X_TDR_TEST_COMPLETE) {
            test_complete = true;
            break;
        }

        /* Otherwise wait for the timeout period / 10 */
        else {
            dev->callbacks->callback_delay_ms(dev->config.timeout / 10, dev->callback_context);
        }
    }

    /* Return if the test isn't over after the timeout */
    if (!test_complete) {
        status = PHY_TIMEOUT;
        PHY_CHECK_END(status);
    }

    /* Read register 3.FEDB[9:0] VCT distance */
    vct_distance = (reg_data & PHY_88Q211X_TDR_VCT_DISTANCE_MASK) >> PHY_88Q211X_TDR_VCT_DISTANCE_SHIFT;

    /* Read register 3.FEDC[7] Polarity */
    status = PHY_READ_REG(PHY_88Q211X_DEV_TDR_STATUS_2, PHY_88Q211X_REG_TDR_STATUS_2, &reg_data);
    PHY_CHECK_END(status);
    vct_polarity = (bool) (reg_data & PHY_88Q211X_TDR_VCT_POLARITY);

    /* Read register 3.FEDC[6:0] VCT Amplitude */
    vct_amplitude = (reg_data & PHY_88Q211X_TDR_VCT_AMPLITUDE_MASK) >> PHY_88Q211X_TDR_VCT_AMPLITUDE_SHIFT;

    /* Interpret the data */
    if (vct_polarity && (vct_amplitude == 0)) {
        *cable_state = PHY_CABLE_STATE_88Q211X_TERMINATED;
    } else if (vct_polarity && (vct_amplitude != 0)) {
        *cable_state = PHY_CABLE_STATE_88Q211X_OPEN;
    } else if (!vct_polarity && (vct_amplitude != 0)) {
        *cable_state = PHY_CABLE_STATE_88Q211X_SHORT;
    } else {
        *cable_state = PHY_CABLE_STATE_88Q211X_ERROR;
    }

    /* Check if the distance measurement is invalid */
    if ((vct_distance < 40) && (vct_distance > 1023)) {
        *maximum_peak_distance = UINT32_MAX;
    }

    /* If it is valid then compute the distance in mm */
    else {
        *maximum_peak_distance = (vct_distance * 129.31) - 5043.09;
    }

end:
    PHY_UNLOCK;
    return status;
}
