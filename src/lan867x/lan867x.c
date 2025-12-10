/*
 * lan867x.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "lan867x.h"
#include "internal/phy_utils.h"
#include "internal/lan867x/lan867x_init.h"
#include "internal/lan867x/lan867x_regs.h"


phy_status_t PHY_LAN867X_ProcessInterrupt(phy_handle_lan867x_t *dev) {

    phy_status_t status       = PHY_OK;
    uint16_t     reg_data     = 0;
    bool         source_found = false;

    PHY_LOCK;

    /* Check status register 1 */
    {
        status = PHY_READ_REG(PHY_LAN867X_DEV_STS1, PHY_LAN867X_REG_STS1, &reg_data);
        PHY_CHECK_END(status);

        /* Signal Quality Indication Status */
        if (reg_data & LAN867X_MISC_SQI) {
            source_found = true;
        }

        /* PLCA Status Changed */
        if (reg_data & LAN867X_MISC_PSTC) {
            source_found = true;
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
        // status = PHY_READ_REG(PHY_LAN867X_DEV_STS3, PHY_LAN867X_REG_STS3, &reg_data);
        // PHY_CHECK_END(status);
    }

    /* Check status register 2 */
    {
        status = PHY_READ_REG(PHY_LAN867X_DEV_STS2, PHY_LAN867X_REG_STS2, &reg_data);
        PHY_CHECK_END(status);

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


phy_status_t PHY_LAN867X_EnableIEEEPowerDown(phy_handle_lan867x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the basic control register */
    status = PHY_READ_REG(PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, &reg_data);
    PHY_CHECK_END(status);

    /* Set the power down bit if it isn't set */
    if (!(reg_data & PHY_LAN867X_PD)) {
        reg_data |= PHY_LAN867X_PD;
        status    = PHY_WRITE_REG(PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, reg_data);
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
    status = PHY_READ_REG(PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, &reg_data);
    PHY_CHECK_RET(status);

    /* Clear the power down bit if it isn't cleared */
    if (reg_data & PHY_LAN867X_PD) {
        reg_data &= ~PHY_LAN867X_PD;
        status    = PHY_WRITE_REG(PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, reg_data);
        PHY_CHECK_END(status);
    }

end:
    PHY_UNLOCK;
    return status;
}
