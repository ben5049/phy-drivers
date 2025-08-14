/*
 * lan867x.c
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#include "lan867x.h"
#include "internal/phy_utils.h"


phy_status_t PHY_LAN867X_Init(phy_handle_lan867x_t *dev, const phy_config_lan867x_t *config, const phy_callbacks_t *callbacks, void *callback_context){

    PHY_CHECK_HANDLE_MEMBERS(phy_handle_lan867x_t);
    PHY_CHECK_CONFIG_MEMBERS(phy_config_lan867x_t);

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    return status;
}
