/*
 * phy_common.c
 *
 *  Created on: Aug 13, 2025
 *      Author: bens1
 */

#include "phy_common.h"
#include "88q211x.h"
#include "lan867x.h"
#include "dp83867.h"

phy_status_t PHY_Init(void *dev, void *config, const phy_callbacks_t *callbacks, void *callback_context) {

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

        case (PHY_VARIANT_DP83867):
            status = PHY_DP83867_Init(dev, config, callbacks, callback_context);
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}
