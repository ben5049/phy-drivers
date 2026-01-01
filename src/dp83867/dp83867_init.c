/*
 * dp83867_init.c
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#include "internal/phy_utils.h"
#include "internal/phy_io.h"

#include "dp83867.h"
#include "internal/dp83867/dp83867_init.h"
#include "internal/dp83867/dp83867_regs.h"


static phy_status_t PHY_DP83867_CheckID(phy_handle_dp83867_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;
    // uint16_t     reg_data = 0;
    // uint32_t     oui      = 0;
    // uint8_t      model    = 0;

    // TODO: Implement

    return status;
}


static phy_status_t PHY_DP83867_SoftwareReset(phy_handle_dp83867_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    // TODO: Implement

    DP83867_CLEAR_STATE(dev);

    return status;
}


phy_status_t PHY_DP83867_Init(phy_handle_dp83867_t *dev, const phy_config_dp83867_t *config, const phy_callbacks_t *callbacks, void *callback_context) {

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_dp83867_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_dp83867_t);
    PHY_CHECK_EVENTS_MEMBERS(phy_event_counters_dp83867_t);

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR; // TODO: change to PHY_OK when done


    /* Check config parameters. TODO: More */
    if (config->phy_addr > 31) status = PHY_INVALID_PHY_ADDR_ERROR;
    if (config->variant != PHY_VARIANT_DP83867) status = PHY_INVALID_VARIANT_ERROR;
    if ((config->interface != PHY_INTERFACE_MII) && (config->interface != PHY_INTERFACE_GMII) && (config->interface != PHY_INTERFACE_RGMII)) status = PHY_INVALID_INTERFACE_ERROR;
    PHY_CHECK_RET(status);

    /* Check the callbacks */
    if (callbacks->callback_read_reg_c22 == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_write_reg_c22 == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_get_time_ms == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_delay_ms == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_delay_ns == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_take_mutex == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_give_mutex == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_event == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    if (callbacks->callback_write_log == NULL) status = PHY_MISSING_CALLBACK_ERROR;
    PHY_CHECK_RET(status);

    /* Take the mutex */
    status = callbacks->callback_take_mutex(config->timeout, dev->callback_context);
    PHY_CHECK_RET(status);

    /* Assign the inputs */
    dev->config    = *config;
    dev->callbacks = callbacks;

    /* Set fixed attributes */
    dev->speed         = PHY_SPEED_UNKNOWN;
    dev->duplex        = PHY_FULL_DUPLEX;
    dev->autoneg       = true;
    dev->role          = PHY_ROLE_UNKNOWN;
    dev->config.c45_en = false;

    /* Check the ID and get the silicon revision */
    status = PHY_DP83867_CheckID(dev);
    PHY_CHECK_END(status);

    /* Perform a software reset */
    status = PHY_DP83867_SoftwareReset(dev);
    PHY_CHECK_END(status);

    /* TODO:
     *  - Enable autoneg
     *  - Enable temp sensor
     *  - Enable SQI
     *  - Disable output clock
     *  - Enable 1ns clock skew
     */

end:

    /* Release the mutex */
    PHY_UNLOCK;
    return status;
}
