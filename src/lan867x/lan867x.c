/*
 * lan867x.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "lan867x.h"
#include "internal/phy_utils.h"
#include "internal/lan867x/lan867x_regs.h"


/* Proprietary indirect access mechanism (from AN1699 p2, DS60001699G) */
static phy_status_t PHY_LAN867X_IndirectRead(phy_handle_lan867x_t *dev, uint8_t addr, uint8_t mask, uint16_t *data) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_WRITE_REG(PHY_LAN867X_DEV_IR_ADDR, PHY_LAN867X_REG_IR_ADDR, addr);
    PHY_CHECK_RET;
    PHY_WRITE_REG(PHY_LAN867X_DEV_IR_MODE, PHY_LAN867X_REG_IR_MODE, LAN867X_MISC_IR_MODE);
    PHY_CHECK_RET;
    PHY_READ_REG(PHY_LAN867X_DEV_IR_DATA, PHY_LAN867X_REG_IR_DATA, &reg_data);
    PHY_CHECK_RET;

    *data = (reg_data & mask);

    return status;
}

/* Write the optional configuration to the PHY for EMI and compliance (from AN1699 p3, DS60001699G) */
static phy_status_t PHY_LAN867X_ApplyConfig(phy_handle_lan867x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    if (dev->silicon_revision == PHY_LAN867X_SI_REV_C2) {
        status = PHY_NOT_IMPLEMENTED_ERROR;

    } else if (dev->silicon_revision == PHY_LAN867X_SI_REV_D0) {
        PHY_WRITE_REG(0x1f, 0x0037, 0x0800);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x008a, 0xbfc0);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x0118, 0x029c);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x00d6, 0x1001);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x0082, 0x001c);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x00fd, 0x0c0b);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x00fd, 0x8c07);
        PHY_CHECK_RET;
        PHY_WRITE_REG(0x1f, 0x0091, 0x9660);
        PHY_CHECK_RET;

        /* Write to link status control register */
        reg_data  = LAN867X_MISC_LSCTL_LSPSEL_DIS << LAN867X_MISC_LSCTL_LSPSEL_SHIFT;       /* Disable link state output to pin */
        reg_data |= LAN867X_MISC_LSCTL_LSCFG_PLCA_STATUS << LAN867X_MISC_LSCTL_LSCFG_SHIFT; /* Link status is PLCA status */
        reg_data |= LAN867X_MISC_LSCTL_LSTMR_15MS << LAN867X_MISC_LSCTL_LSTMR_SHIFT;        /* Default timeout 15ms */
        PHY_WRITE_REG(PHY_LAN867X_DEV_LSCTL_REV_D, PHY_LAN867X_REG_LSCTL_REV_D, reg_data);
        PHY_CHECK_RET;
    } else {
        status = PHY_NOT_IMPLEMENTED_ERROR;
        // TODO: printf("Unknown silicone revision %d", dev->silicon_revision)
    }

    return status;
}

phy_status_t PHY_LAN867X_Init(phy_handle_lan867x_t *dev, const phy_config_lan867x_t *config, const phy_callbacks_t *callbacks, void *callback_context) {

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_lan867x_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_lan867x_t);

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR; // TODO: Remove

    /* Check config parameters. TODO: More */
    if ((config->variant == PHY_VARIANT_LAN8670) && (config->interface != PHY_INTERFACE_MII) && (config->interface != PHY_INTERFACE_RMII)) status = PHY_PARAMETER_ERROR;
    if ((config->variant == PHY_VARIANT_LAN8671) && (config->interface != PHY_INTERFACE_RMII)) status = PHY_PARAMETER_ERROR;
    if ((config->variant == PHY_VARIANT_LAN8672) && (config->interface != PHY_INTERFACE_MII)) status = PHY_PARAMETER_ERROR;
    if (config->default_speed != PHY_SPEED_10M) status = PHY_PARAMETER_ERROR;
    PHY_CHECK_RET;

    /* Check the callbacks */
    if (callbacks->callback_read_reg == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_write_reg == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_get_time_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ns == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_take_mutex == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_give_mutex == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_link_status_change == NULL) status = PHY_PARAMETER_ERROR;
    PHY_CHECK_RET;

    // TODO:
    // - Check ID
    // - Get silicon revision

    status = PHY_LAN867X_ApplyConfig(dev);
    PHY_CHECK_RET;

    return status;
}
