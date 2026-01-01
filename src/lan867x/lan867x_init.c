/*
 * lan867x_init.c
 *
 *  Created on: Dec 8, 2025
 *      Author: bens1
 */

#include "internal/phy_utils.h"
#include "internal/phy_io.h"

#include "lan867x.h"
#include "internal/lan867x/lan867x_init.h"
#include "internal/lan867x/lan867x_regs.h"


/* Proprietary indirect access mechanism (from AN1699 p2, DS60001699G) */
static phy_status_t PHY_LAN867X_IndirectRead(phy_handle_lan867x_t *dev, uint8_t addr, uint8_t mask, uint16_t *data) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    status = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_IR_ADDR, PHY_LAN867X_REG_IR_ADDR, addr);
    PHY_CHECK_RET(status);
    status = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_IR_MODE, PHY_LAN867X_REG_IR_MODE, LAN867X_MISC_IR_MODE);
    PHY_CHECK_RET(status);
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_IR_DATA, PHY_LAN867X_REG_IR_DATA, &reg_data);
    PHY_CHECK_RET(status);

    *data = (reg_data & mask);

    return status;
}

static phy_status_t PHY_LAN867X_CheckID(phy_handle_lan867x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;
    uint32_t     oui      = 0;
    uint8_t      model    = 0;

    /* Read the first ID register */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_PHY_ID1, PHY_LAN867X_REG_PHY_ID1, &reg_data);
    PHY_CHECK_RET(status);

    /* Get bits 2:17 of the organisationally unique identifier */
    oui |= (uint32_t) reg_data << 6;

    /* Read the second ID register */
    status = PHY_READ_REG(dev, PHY_LAN867X_DEV_PHY_ID2, PHY_LAN867X_REG_PHY_ID2, &reg_data);
    PHY_CHECK_RET(status);

    /* Get bits 18:23 of the organisationally unique identifier and check it is correct */
    oui |= ((reg_data & PHY_LAN867X_OUI_18_23_MASK) >> PHY_LAN867X_OUI_18_23_SHIFT);
    if (oui != PHY_LAN867X_OUI) status = PHY_ID_ERROR;
    PHY_CHECK_RET(status);

    /* Get the model number and check it is correct */
    model = (reg_data & PHY_LAN867X_MODEL_MASK) >> PHY_LAN867X_MODEL_SHIFT;
    if (model != PHY_LAN867X_MODEL_NUMBER) status = PHY_ID_ERROR;
    PHY_CHECK_RET(status);

    /* Get the revision and check it is correct */
    dev->silicon_revision = (reg_data & PHY_LAN867X_REV_MASK) >> PHY_LAN867X_REV_SHIFT;
    if ((dev->silicon_revision == PHY_LAN867X_SI_REV_D0) && (dev->config.variant == PHY_VARIANT_LAN8672)) status = PHY_ID_ERROR;
    PHY_CHECK_RET(status);

    return status;
}

static phy_status_t PHY_LAN867X_SoftwareReset(phy_handle_lan867x_t *dev) {

    phy_status_t status = PHY_OK;

    status = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_BASIC_CONTROL, PHY_LAN867X_REG_BASIC_CONTROL, PHY_LAN867X_SW_RESET);
    PHY_CHECK_RET(status);

    LAN867X_CLEAR_STATE(dev);

    return status;
}

/* Write the configuration to the PHY for EMI and standards compliance, as well as to enable SQI */
static phy_status_t PHY_LAN867X_ApplyConfigEnableSQI(phy_handle_lan867x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    switch (dev->silicon_revision) {

        /* Revision C2 - Configuration Process (AN1699 section 3.1 - DS60001699G) */
        case PHY_LAN867X_SI_REV_C2:

            /* Create variables */
            int8_t   offset1, offset2;
            uint16_t cfgparam1, cfgparam2, cfgparam3, cfgparam4, cfgparam5;

            /* Get the first magic number */
            status = PHY_LAN867X_IndirectRead(dev, 0x04, 0x1f, &reg_data);
            PHY_CHECK_RET(status);
            if ((reg_data & 0x10) != 0) {
                offset1 = (int8_t) ((uint8_t) reg_data - 0x20);
            } else {
                offset1 = (int8_t) reg_data;
            }
            if ((offset1 > 15) || (offset1 < -5)) status = PHY_INVALID_REGISTER_CONTENT_ERROR;
            PHY_CHECK_RET(status);

            /* Get the second magic number */
            status = PHY_LAN867X_IndirectRead(dev, 0x08, 0x1f, &reg_data);
            PHY_CHECK_RET(status);
            if ((reg_data & 0x10) != 0) {
                offset2 = (int8_t) ((uint8_t) reg_data - 0x20);
            } else {
                offset2 = (int8_t) reg_data;
            }

            /* More magic maths */
            cfgparam1 = (uint16_t) (((9 + offset1) & 0x3f) << 10) | (uint16_t) (((14 + offset1) & 0x3f) << 4) | 0x03;
            cfgparam2 = (uint16_t) (((40 + offset2) & 0x3f) << 10);

            /* Write magic config registers */
            status = PHY_WRITE_REG(dev, 0x1f, 0x00d0, 0x3f31);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00e0, 0xc000);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0084, cfgparam1);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x008a, cfgparam2);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00e9, 0x9e50);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00f5, 0x1cf8);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00f4, 0xc020);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00f8, 0xb900);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00f9, 0x4e53);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0081, 0x0080);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0091, 0x9660);
            PHY_CHECK_RET(status);

            /* Even more magic maths for SQI */
            cfgparam3 = (uint16_t) (((5 + offset1) & 0x3f) << 8) | (uint16_t) ((9 + offset1) & 0x3f);
            cfgparam4 = (uint16_t) (((9 + offset1) & 0x3f) << 8) | (uint16_t) ((14 + offset1) & 0x3f);
            cfgparam5 = (uint16_t) (((17 + offset1) & 0x3f) << 8) | (uint16_t) ((22 + offset1) & 0x3f);

            /* Write the magic registers to calibrate SQI */
            status = PHY_WRITE_REG(dev, 0x1f, 0x00ad, cfgparam3);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00ae, cfgparam4);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00af, cfgparam5);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b0, 0x0103);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b1, 0x0910);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b2, 0x1d26);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b3, 0x002a);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b4, 0x0103);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b5, 0x070d);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b6, 0x1720);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b7, 0x0027);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b8, 0x0509);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00b9, 0x0e13);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00ba, 0x1c25);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00bb, 0x002b);
            PHY_CHECK_RET(status);
            break;

        /* Other revisions use D0 config */
        default:
            PHY_LOG("Warning, unknown silicon revision '%d', applying D0 config (potentially unstable)", dev->silicon_revision);

        /* Revision D0 and later - Configuration Process (AN1699 section 2.1 - DS60001699G) */
        case PHY_LAN867X_SI_REV_D0:

            /* Write magic config registers */
            status = PHY_WRITE_REG(dev, 0x1f, 0x0037, 0x0800);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x008a, 0xbfc0);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0118, 0x029c);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00d6, 0x1001);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0082, 0x001c);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00fd, 0x0c0b);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x00fd, 0x8c07);
            PHY_CHECK_RET(status);
            status = PHY_WRITE_REG(dev, 0x1f, 0x0091, 0x9660);
            PHY_CHECK_RET(status);

            /* Write to link status control register */
            reg_data  = LAN867X_MISC_LSCTL_LSPSEL_DIS << LAN867X_MISC_LSCTL_LSPSEL_SHIFT;       /* Disable link state output to pin */
            reg_data |= LAN867X_MISC_LSCTL_LSCFG_PLCA_STATUS << LAN867X_MISC_LSCTL_LSCFG_SHIFT; /* Link status is PLCA status */
            reg_data |= LAN867X_MISC_LSCTL_LSTMR_15MS << LAN867X_MISC_LSCTL_LSTMR_SHIFT;        /* Default timeout 15ms */
            status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_LSCTL_REV_D, PHY_LAN867X_REG_LSCTL_REV_D, reg_data);
            PHY_CHECK_RET(status);

            /* TODO: Enable SQI */

            break;

        /* Old versions with no configuration provided */
        case PHY_LAN867X_SI_REV_A0:
        case PHY_LAN867X_SI_REV_B1:
        case PHY_LAN867X_SI_REV_C1:
            PHY_LOG("Warning, old silicon revision '%d', no config to apply (potentially unstable)", dev->silicon_revision);
            break;
    }

    /* Enable multidrop (newer revisions turn it on by default) */
    if (dev->silicon_revision < PHY_LAN867X_SI_REV_D0) {
        status = PHY_READ_REG(dev, PHY_LAN867X_DEV_PMA_T1SPMACTL, PHY_LAN867X_REG_PMA_T1SPMACTL, &reg_data);
        PHY_CHECK_RET(status);
        reg_data |= PHY_LAN867X_MDE;
        status    = PHY_WRITE_REG(dev, PHY_LAN867X_DEV_PMA_T1SPMACTL, PHY_LAN867X_REG_PMA_T1SPMACTL, reg_data);
        PHY_CHECK_RET(status);
    }

    return status;
}

/* Enable physical layer collision avoidance (PLCA) */
static phy_status_t PHY_LAN867X_PLCAEnable(phy_handle_lan867x_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;


    // TODO: disabling collision detection is recommended

    return status;
}

phy_status_t PHY_LAN867X_Init(phy_handle_lan867x_t *dev, const phy_config_lan867x_t *config, const phy_callbacks_t *callbacks, void *callback_context) {

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_lan867x_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_lan867x_t);
    PHY_CHECK_EVENTS_MEMBERS(phy_event_counters_lan867x_t);

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR; // TODO: change to PHY_OK when done

    /* Check config parameters. TODO: More */
    if ((config->variant != PHY_VARIANT_LAN8670) && (config->variant != PHY_VARIANT_LAN8671) && (config->variant != PHY_VARIANT_LAN8672)) status = PHY_PARAMETER_ERROR;
    if ((config->variant == PHY_VARIANT_LAN8670) && (config->interface != PHY_INTERFACE_MII) && (config->interface != PHY_INTERFACE_RMII)) status = PHY_PARAMETER_ERROR;
    if ((config->variant == PHY_VARIANT_LAN8671) && (config->interface != PHY_INTERFACE_RMII)) status = PHY_PARAMETER_ERROR;
    if ((config->variant == PHY_VARIANT_LAN8672) && (config->interface != PHY_INTERFACE_MII)) status = PHY_PARAMETER_ERROR;
    if (config->plca_enabled && (config->plca_id == 0) && (config->plca_node_count == 0)) status = PHY_PARAMETER_ERROR; /* There must be at least on transmit opportunity */
    PHY_CHECK_RET(status);

    /* Check the callbacks */
    if (callbacks->callback_read_reg_c22 == NULL) status = PHY_PARAMETER_ERROR; /* Only clause 22 access is used */
    if (callbacks->callback_write_reg_c22 == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_get_time_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ms == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_delay_ns == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_take_mutex == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_give_mutex == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_event == NULL) status = PHY_PARAMETER_ERROR;
    if (callbacks->callback_write_log == NULL) status = PHY_PARAMETER_ERROR;
    PHY_CHECK_RET(status);

    /* Take the mutex */
    status = callbacks->callback_take_mutex(config->timeout, dev->callback_context);
    PHY_CHECK_RET(status);

    /* Assign the inputs */
    dev->config    = *config;
    dev->callbacks = callbacks;

    /* Set fixed attributes */
    dev->speed         = PHY_SPEED_10M;
    dev->duplex        = PHY_HALF_DUPLEX;
    dev->autoneg       = false;
    dev->role          = PHY_ROLE_UNKNOWN;
    dev->config.c45_en = false;

    /* Check the ID and get the silicon revision */
    status = PHY_LAN867X_CheckID(dev);
    PHY_CHECK_END(status);

    /* Perform a software reset */
    status = PHY_LAN867X_SoftwareReset(dev);
    PHY_CHECK_END(status);

    /* Apply the arcane config and enable SQI monitoring */
    status = PHY_LAN867X_ApplyConfigEnableSQI(dev);
    PHY_CHECK_END(status);

    /* Enable PLCA (should normally be on) */
    if (dev->config.plca_enabled) {
        status = PHY_LAN867X_PLCAEnable(dev);
        PHY_CHECK_END(status);
    }

end:

    /* Release the mutex */
    PHY_UNLOCK;
    return status;
}
