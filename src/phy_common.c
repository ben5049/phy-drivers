/*
 * phy_common.c
 *
 *  Created on: Aug 13, 2025
 *      Author: bens1
 */

#include "phy_common.h"
#include "88q211x.h"
#include "lan867x.h"
#include "internal/phy_utils.h"


phy_status_t PHY_Init(void *dev, void *config, phy_callbacks_t *callbacks, void *callback_context) {

    phy_status_t status = PHY_OK;

    switch (((phy_config_base_t *) config)->variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_Init(dev, config, callbacks, callback_context);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_Init(dev, config, callbacks, callback_context);
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}


void PHY_GetDuplex(void *dev, phy_duplex_t *duplex) {
    *duplex = ((phy_handle_base_t *) dev)->duplex;
}


void PHY_GetSpeed(void *dev, phy_speed_t *speed) {
    *speed = ((phy_handle_base_t *) dev)->speed;
}
