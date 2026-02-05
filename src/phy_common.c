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


phy_status_t PHY_ProcessInterrupt(void *dev) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_ProcessInterrupt(dev);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_ProcessInterrupt(dev);
            break;

        case (PHY_VARIANT_DP83867):
            status = PHY_NOT_IMPLEMENTED_ERROR;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}


phy_status_t PHY_GetLinkState(void *dev, bool *linkup) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_GetLinkState(dev, linkup);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            *linkup = false; /* 10BASE-T1S PHYs have no concept of link state */
            break;

        case (PHY_VARIANT_DP83867):
            status = PHY_DP83867_GetLinkState(dev, linkup);
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}
