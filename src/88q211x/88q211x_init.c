/*
 * 88q211x_init.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/88q211x/88q211x_init.h"
#include "internal/88q211x/88q211x_regs.h"
#include "internal/phy_utils.h"


static void PHY_88Q211X_ResetEventCounters(phy_handle_88q211x_t *dev) {
}


static phy_status_t PHY_88Q211X_CheckID(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;
    uint32_t     oui      = 0;
    uint8_t      model    = 0;
    uint8_t      revision = 0;

    /* Read the first ID register */
    PHY_READ_REG(PHY_88Q211X_DEV_PMA_PMD_DEV_ID_1, PHY_88Q211X_REG_PMA_PMD_DEV_ID_1, &reg_data);
    PHY_CHECK_RET;

    /* Get bits 3:18 of the organisationally unique identifier */
    oui |= (uint32_t) ((reg_data & PHY_88Q211X_OUI_3_18_MASK) >> PHY_88Q211X_OUI_3_18_SHIFT) << 6;

    /* Read the second ID register */
    PHY_READ_REG(PHY_88Q211X_DEV_PMA_PMD_DEV_ID_2, PHY_88Q211X_REG_PMA_PMD_DEV_ID_2, &reg_data);
    PHY_CHECK_RET;

    /* Get bits 19:24 of the organisationally unique identifier and check it is correct */
    oui |= (reg_data & PHY_88Q211X_OUI_19_24_MASK) >> PHY_88Q211X_OUI_19_24_SHIFT;
    if (oui != PHY_88Q211X_OUI) status = PHY_ID_ERROR;
    PHY_CHECK_RET;

    /* Get the model number and check it is correct */
    model = (reg_data & PHY_88Q211X_MODEL_NUMBER_MASK) >> PHY_88Q211X_MODEL_NUMBER_SHIFT;
    if (model != PHY_88Q211X_MODEL_NUMBER) status = PHY_ID_ERROR;
    PHY_CHECK_RET;

    /* Get the revision number (only for debug purposes) */
    revision = (reg_data & PHY_88Q211X_REVISION_NUMBER_MASK) >> PHY_88Q211X_REVISION_NUMBER_SHIFT;
    PHY_UNUSED(revision);

    return status;
}


static phy_status_t PHY_88Q211X_SoftwareResetRGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;
    uint16_t     reg_data;

    /* Read the reset and control register */
    PHY_READ_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, &reg_data);
    PHY_CHECK_RET;

    /* Set the RGMII reset bit */
    reg_data |= PHY_88Q211X_RST_RGMII;

    /* Write the reset and control register */
    PHY_READ_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, reg_data);
    PHY_CHECK_RET;

    /* The RGMII reset bit is not self clearing so unset the RGMII reset bit */
    reg_data &= ~PHY_88Q211X_RST_RGMII;

    /* Write the reset and control register */
    PHY_READ_REG(PHY_88Q211X_DEV_RST_CTRL, PHY_88Q211X_REG_RST_CTRL, reg_data);
    PHY_CHECK_RET;

    return status;
}


static phy_status_t PHY_88Q211X_ConfigureRGMII(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;
    bool         write_back = false;

    /* Get the current RGMII settings */
    PHY_READ_REG(PHY_88Q211X_DEV_RGMII_COM_PORT, PHY_88Q211X_REG_RGMII_COM_PORT, &reg_data);
    PHY_CHECK_RET;

    /* If the new tx clk dely setting is different from the old one then change it and call for a reset */
    if (dev->config.tx_clk_internal_delay && !(reg_data & PHY_88Q211X_RGMII_TX_CLK_INTERNAL_DELAY)) {
        reg_data   |= PHY_88Q211X_RGMII_TX_CLK_INTERNAL_DELAY;
        write_back  = true;
    }

    /* If the new rx clk dely setting is different from the old one then change it and call for a reset */
    if (dev->config.rx_clk_internal_delay && !(reg_data & PHY_88Q211X_RGMII_RX_CLK_INTERNAL_DELAY)) {
        reg_data   |= PHY_88Q211X_RGMII_RX_CLK_INTERNAL_DELAY;
        write_back  = true;
    }

    /* Only write back if required */
    if (write_back) {
        PHY_WRITE_REG(PHY_88Q211X_DEV_RGMII_COM_PORT, PHY_88Q211X_REG_RGMII_COM_PORT, reg_data);
        PHY_CHECK_RET;

        /* Reset for the change to take effect */
        status = PHY_88Q211X_SoftwareResetRGMII(dev);
        PHY_CHECK_RET;
    }

    return status;
}


phy_status_t PHY_88Q211X_Init(phy_handle_88q211x_t *dev, const phy_config_88q211x_t *config, const phy_callbacks_t *callbacks, void *callback_context) {

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_88q211x_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_88q211x_t);

    phy_status_t status = PHY_OK;

    /* Check the device hasn't already been initialised. Note this may cause an unintended error if the struct uses non-zeroed memory. */
    if (dev->state != PHY_STATE_88Q211X_UNCONFIGURED) status = PHY_ALREADY_CONFIGURED;
    PHY_CHECK_RET;

    /* Take the mutex */
    status = callbacks->callback_take_mutex(config->timeout, dev->callback_context);
    PHY_CHECK_RET;

    /* Check config parameters. TODO: More */
    if ((config->variant == PHY_VARIANT_88Q2111) && (config->interface == PHY_INTERFACE_SGMII)) status = PHY_PARAMETER_ERROR;
    PHY_CHECK_END;

    /* Check the callbacks */
    if (callbacks->callback_read_reg == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_write_reg == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_get_time_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ns == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_take_mutex == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_give_mutex == NULL) status = PHY_PARAMETER_ERROR;
    PHY_CHECK_END;

    /* Assign the inputs */
    dev->config    = *config;
    dev->callbacks = callbacks;

    /* Reset parameters */
    dev->speed            = PHY_SPEED_UNKNOWN;
    dev->role             = PHY_ROLE_UNKNOWN;
    dev->autoneg          = false;
    dev->duplex           = PHY_FULL_DUPLEX;
    dev->callback_context = callback_context;
    PHY_88Q211X_ResetEventCounters(dev);


    /* TODO:
     * - Set LP state, PMA/PMD reset, disable loopback, get speed 0x0000
     * - PMA Transmit disable 0x0009
     * - Check speed ability 0x0012
     * - Set speed 0x0834
     * - Set master/slave 0x0834 (bit 7.0202.12?)
     * - Disable 100BASE-T1 test 0x0836 (already disabled)
     * - Reset, low power, tx disable 0x0900
     * - Check eee + LP ability, check RX PMA/PMD link status 0x0901
     * - Enable EEE and OAM advertisement 0x0902 (already enabled)
     * - Check partner EEE and OAM advertisement 0x0903
     * - Test disable 0x0904
     */

    /* Check the PHY ID */
    status = PHY_88Q211X_CheckID(dev);
    PHY_CHECK_END;

    /* TODO: Check speed & mode */

    /* Configure RGMII timing */
    if (dev->config.interface == PHY_INTERFACE_RGMII) {
        status = PHY_88Q211X_ConfigureRGMII(dev);
        PHY_CHECK_END;
    }

    /* TODO: Set the fifo size */


    /* Move from unconfigured to IDLE */
    dev->state = PHY_STATE_88Q211X_IDLE;

end:

    /* Release the mutex */
    PHY_UNLOCK;
    return status;
}
