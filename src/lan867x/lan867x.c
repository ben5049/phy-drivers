/*
 * lan867x.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "phy_utils.h"
#include "phy_io.h"

#include "lan867x.h"
#include "lan867x_init.h"
#include "lan867x_regs.h"


phy_status_t PHY_LAN867X_ProcessInterrupt(phy_handle_lan867x_t *dev) {

    phy_status_t status       = PHY_OK;
    uint16_t     reg_data     = 0;
    bool         source_found = false;

    PHY_LOCK;

    /* Check status register 1 */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_STS1, PHY_LAN867X_REG_STS1, &reg_data);
    PHY_CHECK_END(status);
    if (reg_data) {

        /* Signal Quality Indication Status */
        if (reg_data & LAN867X_MISC_SQI) {
            source_found = true;
        }

        /* PLCA Status Changed */
        if (reg_data & LAN867X_MISC_PSTC) {
            source_found = true;
            status       = PHY_LAN867X_GetLinkState(dev, NULL);
            PHY_CHECK_END(status);
        }

        /* Transmit Collision Status */
        if (reg_data & (LAN867X_MISC_TXCOL)) {
            source_found = true;
            dev->events.mdi_errors++;
        }

        /* Transmit Jabber Status */
        if (reg_data & LAN867X_MISC_TXJAB) {
            source_found = true;
            dev->events.mdi_errors++;
        }

        /* Time Synchronization Service Interface Status */
        if (reg_data & LAN867X_MISC_TSSI) {
            source_found = true;
        }

        /* PLCA Empty Cycle Status */
        if (reg_data & LAN867X_MISC_EMPTYC) {
            source_found = true;
        }

        /* Receive in Transmit Opportunity */
        if (reg_data & LAN867X_MISC_RXINTO) {
            source_found = true;
            dev->events.plca_errors++;
        }

        /* Unexpected BEACON Received */
        if (reg_data & LAN867X_MISC_UNEXPB) {
            source_found = true;
            dev->events.plca_errors++;
        }

        /* BEACON Received Before Transmit Opportunity */
        if (reg_data & LAN867X_MISC_BCNBFTO) {
            source_found = true;
            dev->events.plca_errors++;
        }

        /* Unexpected Carrier Sense */
        if (reg_data & LAN867X_MISC_UNCRS) {
            source_found = true;
            dev->events.mdi_errors++;
        }

        /* PLCA Symbols Detected */
        if (reg_data & LAN867X_MISC_PLCASYM) {
            source_found = true;
            dev->events.plca_errors++;
        }

        /* End-of-Stream Delimiter Error */
        if (reg_data & LAN867X_MISC_ESDERR) {
            source_found = true;
            dev->events.mdi_errors++;
        }

        /* 5B Decode Error */
        if (reg_data & LAN867X_MISC_DEC5B) {
            source_found = true;
            dev->events.mdi_errors++;
        }

        // TODO: Re-enable?
        // /* Get the PLCA Error Transmit Opportunity ID */
        // status = PHY_READ_REG(dev, PHY_LAN867X_DEV_STS3, PHY_LAN867X_REG_STS3, &reg_data);
        // PHY_CHECK_END(status);
    }

    /* Check status register 2 */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_STS2, PHY_LAN867X_REG_STS2, &reg_data);
    PHY_CHECK_END(status);
    if (reg_data) {

        /* Reset complete */
        if (reg_data & LAN867X_MISC_RESETC) {
            source_found = true;
            LAN867X_CLEAR_STATE(dev);
        }

        /* MDI Wake-up */
        if (reg_data & LAN867X_MISC_WKEMDI) {
            source_found = true;
        }

        /* WAKE_IN Wake-up */
        if (reg_data & LAN867X_MISC_WKEWI) {
            source_found = true;
        }

        /* 3.3V supply Under-Voltage */
        if (reg_data & LAN867X_MISC_UV33) {
            source_found = true;
            dev->events.env_errors++;
        }

        /* Over-Temperature Error */
        if (reg_data & LAN867X_MISC_OT) {
            source_found = true;
            dev->events.env_errors++;
        }

        /* Inactivity Watchdog Timeout */
        if (reg_data & LAN867X_MISC_IWDTO) {
            source_found = true;
        }
    }

    /* Unknown interrupt */
    if (!source_found) {
        status = PHY_UNKNOWN_INTERRUPT_ERROR;
        PHY_CHECK_END(status);
    }

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_LAN867X_GetLinkState(phy_handle_lan867x_t *dev, bool *linkup) {

    phy_status_t status          = PHY_OK;
    uint16_t     reg_data        = 0;
    bool         linkup_internal = false;

    PHY_LOCK;

    /* 10BASE-T1S PHYs have no concept of link state. Instead use the PLCA status if available.
     * This only works for a PLCA follower since a coordinator will always be transmitting BEACONs. */
    if (dev->config.plca_enabled && (dev->role == PHY_ROLE_SLAVE)) {

        /* Read the PLCA status register */
        status = PHY_READ_REG(dev, PHY_LAN867X_DEV_PLCA_STS, PHY_LAN867X_REG_PLCA_STS, &reg_data);
        PHY_CHECK_END(status);

        /* Extract the PLCA status bit */
        linkup_internal = reg_data & LAN867X_PLCA_PST;
    }

    /* The master should always count as having its link up */
    else if (dev->config.plca_enabled && (dev->role == PHY_ROLE_MASTER)) {
        linkup_internal = true;
    }

    /* No PLCA */
    else {
        linkup_internal = false;
    }

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


phy_status_t PHY_LAN867X_EnableIEEEPowerDown(phy_handle_lan867x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the basic control register */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, &reg_data);
    PHY_CHECK_END(status);

    /* Set the power down bit if it isn't set */
    if (!(reg_data & PHY_LAN867X_PD)) {
        reg_data |= PHY_LAN867X_PD;
        status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, reg_data);
        PHY_CHECK_END(status);
    }

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_LAN867X_DisableIEEEPowerDown(phy_handle_lan867x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the basic control register */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, &reg_data);
    PHY_CHECK_END(status);

    /* Clear the power down bit if it isn't cleared */
    if (reg_data & PHY_LAN867X_PD) {
        reg_data &= ~PHY_LAN867X_PD;
        status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, reg_data);
        PHY_CHECK_END(status);
    }

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_LAN867X_GetSQI(phy_handle_lan867x_t *dev, uint8_t *sqi) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;
    uint8_t      sqi_internal = PHY_SQI_INVALID;

    PHY_LOCK;

    /* Use the legacy SQI measurement */
    if (dev->silicon_revision < PHY_LAN867X_SI_REV_D0) {

        /* Read the SQI status 1 register */
        status = PHY_READ_REG(dev, PHY_LAN867X_DEV_SQISTS0, PHY_LAN867X_REG_SQISTS0, &reg_data);
        PHY_CHECK_END(status);

        /* An error occured */
        if (reg_data & LAN867X_SQI_ERR) {

            /* Set the output to invalid */
            sqi_internal = PHY_SQI_INVALID;

            // TODO: extract the error code?

            /* Reset the SQI block */
            status = PHY_READ_REG(dev, PHY_LAN867X_DEV_SQICTL, PHY_LAN867X_REG_SQICTL, &reg_data);
            PHY_CHECK_RET(status);
            reg_data &= ~LAN867X_SQI_EN;
            status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_SQICTL, PHY_LAN867X_REG_SQICTL, reg_data);
            PHY_CHECK_RET(status);
            reg_data |= LAN867X_SQI_EN;
            status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_SQICTL, PHY_LAN867X_REG_SQICTL, reg_data);
            PHY_CHECK_RET(status);
        }

        /* The reading is valid, normalise to be between 0 and 100 */
        else if (reg_data & LAN867X_SQI_VLD) {
            sqi_internal = (reg_data & LAN867X_SQI_VAL_MASK) >> LAN867X_SQI_VAL_SHIFT;
            sqi_internal = ((uint16_t) sqi_internal * 100) / 7;
        }

        /* No error, also not valid */
        else {
            sqi_internal = PHY_SQI_INVALID;
        }
    }

    /* Use the dynamic channel quality SQI */
    else {

        /* Read the signal quality index plus register */
        status = PHY_READ_REG(dev, PHY_LAN867X_DEV_DCQ_SQIP_REV_D, PHY_LAN867X_REG_DCQ_SQIP_REV_D, &reg_data);
        PHY_CHECK_RET(status);

        /* 32-level measurement ready */
        if (reg_data & LAN867X_DCQ_SQIP_UDP) {
            sqi_internal = (reg_data & LAN867X_DCQ_SQIP_MASK) >> LAN867X_DCQ_SQIP_SHIFT;
            sqi_internal = ((uint16_t) sqi_internal * 100) / 31;
        }

        /* Read the signal quality index register */
        status = PHY_READ_REG(dev, PHY_LAN867X_DEV_DCQ_SQI_REV_D, PHY_LAN867X_REG_DCQ_SQI_REV_D, &reg_data);
        PHY_CHECK_RET(status);

        /* No 32-level measurement, but 8-level measurement is ready */
        if ((sqi_internal == PHY_SQI_INVALID) && (reg_data & LAN867X_DCQ_SQI_UDP)) {
            sqi_internal = (reg_data & LAN867X_DCQ_SQI_MASK) >> LAN867X_DCQ_SQI_SHIFT;
            sqi_internal = ((uint16_t) sqi_internal * 100) / 7;
        }
    }

end:

    *sqi = sqi_internal;

    PHY_UNLOCK;
    return status;
}
