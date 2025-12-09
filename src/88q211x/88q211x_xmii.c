/*
 * 88q211x_init.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/88q211x/88q211x_regs.h"
#include "internal/88q211x/88q211x_xmii.h"
#include "internal/phy_utils.h"


phy_status_t PHY_88Q211X_SoftwareResetRGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    /* Read the reset and control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Set the RGMII reset bit */
    reg_data |= PHY_88Q211X_RST_RGMII;

    /* Write the reset and control register */
    status = PHY_WRITE_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, reg_data);
    PHY_CHECK_RET(status);

    /* The RGMII reset bit is not self clearing so unset the RGMII reset bit */
    reg_data &= ~PHY_88Q211X_RST_RGMII;

    /* Write the reset and control register */
    status = PHY_WRITE_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, reg_data);
    PHY_CHECK_RET(status);

    return status;
}


phy_status_t PHY_88Q211X_ConfigureRGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status     = PHY_OK;
    uint16_t     reg_data   = 0;
    bool         write_back = false;

    /* Power down SGMII */
    status = PHY_88Q211X_PowerDownSGMII(dev, true);
    PHY_CHECK_RET(status);

    /* Get the current RGMII settings */
    status = PHY_READ_REG(PHY_88Q211X_DEV_RGMII_COM_PORT, PHY_88Q211X_REG_RGMII_COM_PORT, &reg_data);
    PHY_CHECK_RET(status);

    /* If the new tx clk delay setting is different from the old one then change it and call for a reset */
    if (dev->config.tx_clk_internal_delay && !(reg_data & PHY_88Q211X_RGMII_TX_CLK_INTERNAL_DELAY)) {
        reg_data   |= PHY_88Q211X_RGMII_TX_CLK_INTERNAL_DELAY;
        write_back  = true;
    }

    /* If the new rx clk delay setting is different from the old one then change it and call for a reset */
    if (dev->config.rx_clk_internal_delay && !(reg_data & PHY_88Q211X_RGMII_RX_CLK_INTERNAL_DELAY)) {
        reg_data   |= PHY_88Q211X_RGMII_RX_CLK_INTERNAL_DELAY;
        write_back  = true;
    }

    /* Only write back if required */
    if (write_back) {
        status = PHY_WRITE_REG(PHY_88Q211X_DEV_RGMII_COM_PORT, PHY_88Q211X_REG_RGMII_COM_PORT, reg_data);
        PHY_CHECK_RET(status);

        /* Reset for the change to take effect */
        status = PHY_88Q211X_SoftwareResetRGMII(dev);
        PHY_CHECK_RET(status);
    }

    return status;
}


phy_status_t PHY_88Q211X_SoftwareResetSGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    /* Reset SGMII */
    // TODO: Use enums and macros for registers
    status = PHY_READ_REG(0x04, 0x8000, &reg_data);
    PHY_CHECK_RET(status);
    reg_data &= ~0x8000;
    status    = PHY_WRITE_REG(0x04, 0x8000, reg_data);
    PHY_CHECK_RET(status);
    reg_data |= 0x8000;
    status    = PHY_WRITE_REG(0x04, 0x8000, reg_data);
    PHY_CHECK_RET(status);

    return status;
}


phy_status_t PHY_88Q211X_ConfigureSGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    // status = PHY_88Q211X_PowerDownSGMII(dev, false);

    return status;
}


phy_status_t PHY_88Q211X_PowerDownSGMII(phy_handle_88q211x_t *dev, bool enable) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    /* Read the SGMII control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_FIBER_CTRL, PHY_88Q211X_REG_FIBER_CTRL, &reg_data);
    PHY_CHECK_RET(status);

    /* Power down if required */
    if (enable && !(reg_data & PHY_88Q211X_SGMII_POWER_DOWN)) {
        reg_data |= PHY_88Q211X_SGMII_POWER_DOWN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_FIBER_CTRL, PHY_88Q211X_REG_FIBER_CTRL, reg_data);
        PHY_CHECK_RET(status);
    }

    /* Wake up if required */
    else if (!enable && (reg_data & PHY_88Q211X_SGMII_POWER_DOWN)) {
        reg_data |= PHY_88Q211X_SGMII_POWER_DOWN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_FIBER_CTRL, PHY_88Q211X_REG_FIBER_CTRL, reg_data);
        PHY_CHECK_RET(status);
    }

    return status;
}
