/*
 * 88q211x_init.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "memory.h"

#include "88q211x.h"
#include "internal/88q211x/88q211x_init.h"
#include "internal/88q211x/88q211x_regs.h"
#include "internal/88q211x/88q211x_bist.h"
#include "internal/88q211x/88q211x_xmii.h"
#include "internal/phy_utils.h"


static inline void PHY_88Q211X_ResetEventCounters(phy_handle_88q211x_t *dev) {
    memset(&dev->events, 0, sizeof(phy_event_counters_88q211x_t));
}


static phy_status_t PHY_88Q211X_CheckID(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;
    uint32_t     oui      = 0;
    uint8_t      model    = 0;
    uint8_t      revision = 0;

    /* Read the first ID register */
    PHY_READ_REG(PHY_88Q211X_DEV_PMA_PMD_DEV_ID_1, PHY_88Q211X_REG_PMA_PMD_DEV_ID_1, &reg_data);
    PHY_CHECK_RET;

    /* Get bits 3:18 of the organisationally unique identifier */
    oui |= (uint32_t) ((reg_data & PHY_88Q211X_OUI_3_18_MASK) >> PHY_88Q211X_OUI_3_18_SHIFT) << 3;

    /* Read the second ID register */
    PHY_READ_REG(PHY_88Q211X_DEV_PMA_PMD_DEV_ID_2, PHY_88Q211X_REG_PMA_PMD_DEV_ID_2, &reg_data);
    PHY_CHECK_RET;

    /* Get bits 19:24 of the organisationally unique identifier and check it is correct */
    oui |= ((reg_data & PHY_88Q211X_OUI_19_24_MASK) >> PHY_88Q211X_OUI_19_24_SHIFT) << 19;
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


static phy_status_t PHY_88Q211X_SoftwareResetCopper(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    /* Reset the PMA and PMD control register */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_RET;

    /* Set the reset bit (self clearing) */
    reg_data |= PHY_88Q211X_PMA_PMD_RST;
    PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
    PHY_CHECK_RET;

    /* Reset other copper */
    // PHY_READ_REG(0x03, 0x0900, &reg_data);
    // PHY_CHECK_RET;
    // reg_data |= 0x8000;
    // PHY_WRITE_REG(0x03, 0x0900, reg_data);
    // PHY_CHECK_RET;

    /* Reset other other copper */
    // PHY_READ_REG(0x07, 0x0200, &reg_data);
    // PHY_CHECK_RET;
    // reg_data |= 0x8000;
    // PHY_WRITE_REG(0x07, 0x0200, reg_data);
    // PHY_CHECK_RET;

    return status;
}


static phy_status_t PHY_88Q211X_SetFifoSize(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    if (dev->config.interface == PHY_INTERFACE_RGMII) {

        /* Set the 100BASE-T1 FIFO size */
        {
            /* Get the current FIFO size */
            PHY_READ_REG(PHY_88Q211X_DEV_MAC_CTRL, PHY_88Q211X_REG_MAC_CTRL, &reg_data);
            PHY_CHECK_RET;

            /* If the new fifo size is different from the old one then change it */
            if (((reg_data & PHY_88Q211X_CU_TX_FIFO_DEPTH_MASK) >> PHY_88Q211X_CU_TX_FIFO_DEPTH_SHIFT) != dev->config.fifo_size) {
                reg_data &= ~PHY_88Q211X_CU_TX_FIFO_DEPTH_MASK;
                reg_data |= ((uint16_t) dev->config.fifo_size << PHY_88Q211X_CU_TX_FIFO_DEPTH_SHIFT) & PHY_88Q211X_CU_TX_FIFO_DEPTH_MASK;

                /* Write the data back */
                PHY_WRITE_REG(PHY_88Q211X_DEV_MAC_CTRL, PHY_88Q211X_REG_MAC_CTRL, reg_data);
                PHY_CHECK_RET;
            }
        }

        /* Set the 1000BASE-T1 FIFO size (3.FD20.1:0) */
        {
            /* Get the current FIFO size */
            PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T1_TX_FIFO_CTRL, PHY_88Q211X_REG_1000BASE_T1_TX_FIFO_CTRL, &reg_data);
            PHY_CHECK_RET;

            /* If the new fifo size is different from the old one then change it */
            if (((reg_data & PHY_88Q211X_FIBER_TX_FIFO_DEPTH_MASK) >> PHY_88Q211X_FIBER_TX_FIFO_DEPTH_SHIFT) != dev->config.fifo_size) {
                reg_data &= ~PHY_88Q211X_FIBER_TX_FIFO_DEPTH_MASK;
                reg_data |= ((uint16_t) dev->config.fifo_size << PHY_88Q211X_FIBER_TX_FIFO_DEPTH_SHIFT) & PHY_88Q211X_FIBER_TX_FIFO_DEPTH_MASK;

                /* Write the data back */
                PHY_WRITE_REG(PHY_88Q211X_DEV_MAC_CTRL, PHY_88Q211X_REG_MAC_CTRL, reg_data);
                PHY_CHECK_RET;
            }
        }
    }

    /* TODO: Set the SGMII FIFO size (4.8010.15:14) */
    else if (dev->config.interface == PHY_INTERFACE_SGMII) {
        status = PHY_NOT_IMPLEMENTED_ERROR;
        PHY_CHECK_RET;
    }

    /* Invalid interface */
    else {
        status = PHY_PARAMETER_ERROR;
        PHY_CHECK_RET;
    }

    return status;
}


phy_status_t PHY_88Q211X_Init(phy_handle_88q211x_t *dev, const phy_config_88q211x_t *config, const phy_callbacks_t *callbacks, void *callback_context) {

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_88q211x_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_88q211x_t);

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    /* Check the device hasn't already been initialised. Note this may cause an unintended error if the struct uses non-zeroed memory. */
    if (dev->state != PHY_STATE_88Q211X_UNCONFIGURED) status = PHY_ALREADY_CONFIGURED;
    PHY_CHECK_RET;

    /* Check config parameters. TODO: More */
    if ((config->variant == PHY_VARIANT_88Q2110) && (config->interface == PHY_INTERFACE_SGMII)) status = PHY_PARAMETER_ERROR;
    if (config->default_role >= PHY_ROLE_INVALID) status = PHY_PARAMETER_ERROR;
    if ((config->default_speed != PHY_SPEED_100M) && (config->default_speed != PHY_SPEED_1G)) status = PHY_PARAMETER_ERROR;
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

    /* Take the mutex */
    status = callbacks->callback_take_mutex(config->timeout, dev->callback_context);
    PHY_CHECK_RET;

    /* Assign the inputs */
    dev->config    = *config;
    dev->callbacks = callbacks;

    /* Reset parameters */
    dev->speed               = PHY_SPEED_UNKNOWN;
    dev->role                = PHY_ROLE_UNKNOWN;
    dev->autoneg             = false;
    dev->duplex              = PHY_FULL_DUPLEX;
    dev->linkup              = false;
    dev->temp_sensor_enabled = false;
    dev->callback_context    = callback_context;
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

    /* Reset the copper circuits */
    status = PHY_88Q211X_SoftwareResetCopper(dev);
    PHY_CHECK_END;

    /* Configure interfaces */
    if (dev->config.interface == PHY_INTERFACE_RGMII) {
        status = PHY_88Q211X_ConfigureRGMII(dev);
    } else if (dev->config.interface == PHY_INTERFACE_SGMII) {
        status = PHY_88Q211X_ConfigureSGMII(dev);
    } else {
        status = PHY_PARAMETER_ERROR;
    }
    PHY_CHECK_END;

    /* Set the fifo size */
    status = PHY_88Q211X_SetFifoSize(dev);
    PHY_CHECK_END;

    /* Reset GMII steering (disable loopback, enable packet checker, disable packet generator) */
    status = PHY_88Q211X_ResetGMIISteering(dev);
    PHY_CHECK_END;

    /* Check speed & mode */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &reg_data);
    PHY_CHECK_END;
    dev->role  = (reg_data & PHY_88Q211X_MASTER) ? PHY_ROLE_MASTER : PHY_ROLE_SLAVE;
    dev->speed = (((reg_data & PHY_88Q211X_SPEED_MASK) >> PHY_88Q211X_SPEED_SHIFT) == PHY_88Q211X_SPEED_100M) ? PHY_SPEED_100M : PHY_SPEED_1G; /* TODO: Also check for invalid speed */

    /* Set the port role */
    if ((dev->role != dev->config.default_role) && (dev->config.default_role != PHY_ROLE_UNKNOWN)) {
        status = PHY_88Q211X_SetRole(dev, dev->config.default_role);
        PHY_CHECK_END;
    }

    /* Set the port speed */
    if ((dev->speed != dev->config.default_speed) && (dev->config.default_speed != PHY_SPEED_UNKNOWN)) {
        status = PHY_88Q211X_SetSpeed(dev, dev->config.default_speed);
        PHY_CHECK_END;
    }

    /* Enable polarity correction (for 100BASE-T1) */
    PHY_READ_REG(PHY_88Q211X_DEV_100BASE_T1_CU_CTRL, PHY_88Q211X_REG_100BASE_T1_CU_CTRL, &reg_data);
    PHY_CHECK_END;
    reg_data |= PHY_88Q211X_100BASE_T1_POL_CORRECTION;
    PHY_WRITE_REG(PHY_88Q211X_DEV_100BASE_T1_CU_CTRL, PHY_88Q211X_REG_100BASE_T1_CU_CTRL, reg_data);
    PHY_CHECK_END;

    /* Move from unconfigured to IDLE */
    dev->state = PHY_STATE_88Q211X_IDLE;

end:

    /* Release the mutex */
    PHY_UNLOCK;
    return status;
}
