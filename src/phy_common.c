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


phy_status_t PHY_Init(void *dev, const void *config, const phy_callbacks_t *callbacks, void *callback_context) {

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


phy_status_t PHY_EnableInterrupts(void *dev) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_EnableInterrupts(dev);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_EnableInterrupts(dev);
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
            status = PHY_LAN867X_GetLinkState(dev, linkup);
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


/* SQI from 0 to 100 */
phy_status_t PHY_GetSQI(void *dev, uint8_t *sqi) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_GetSQI(dev, sqi);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_GetSQI(dev, sqi);
            break;

        case (PHY_VARIANT_DP83867):
            *sqi   = PHY_SQI_INVALID;
            status = PHY_OK; /* TODO: implement */
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}


phy_status_t PHY_EnableTemperatureSensor(void *dev) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_EnableTemperatureSensor(dev);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            break; /* No temperature sensor available on this PHY */

        case (PHY_VARIANT_DP83867):
            status = PHY_NOT_IMPLEMENTED_ERROR;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}


phy_status_t PHY_ReadTemperature(void *dev, float *temp, bool *valid) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_ReadTemperature(dev, temp, valid);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            *valid = false; /* No temperature sensor available on this PHY */
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


phy_status_t PHY_Sleep(void *dev) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_EnableIEEEPowerDown(dev);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_EnableIEEEPowerDown(dev);
            break;

        case (PHY_VARIANT_DP83867):
            status = PHY_OK; /* TODO: implement */
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}


phy_status_t PHY_Wake(void *dev) {

    phy_status_t status = PHY_OK;

    switch (((phy_handle_base_t *) dev)->config.variant) {

        case (PHY_VARIANT_88Q2110):
        case (PHY_VARIANT_88Q2112):
            status = PHY_88Q211X_DisableIEEEPowerDown(dev);
            break;

        case (PHY_VARIANT_LAN8670):
        case (PHY_VARIANT_LAN8671):
        case (PHY_VARIANT_LAN8672):
            status = PHY_LAN867X_DisableIEEEPowerDown(dev);
            break;

        case (PHY_VARIANT_DP83867):
            status = PHY_OK; /* TODO: implement */
            break;

        default:
            status = PHY_PARAMETER_ERROR;
            break;
    }

    return status;
}
