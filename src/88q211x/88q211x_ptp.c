/*
 * 88q211x_ptp.c
 *
 *  Created on: Jul 4, 2026
 *      Author: bens1
 */

#include "88q211x.h"
#include "88q211x_ptp.h"


phy_status_t PHY_88Q211X_GetIngressLatency(phy_handle_88q211x_t *dev, uint16_t *ns) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    switch (dev->speed) {

        case PHY_SPEED_100M:
            static_assert(PHY_88Q211X_100M_INGRESS_LATENCY < UINT16_MAX);
            *ns = PHY_88Q211X_100M_INGRESS_LATENCY;
            break;

        case PHY_SPEED_1G:
            static_assert(PHY_88Q211X_1G_INGRESS_LATENCY < UINT16_MAX);
            *ns = PHY_88Q211X_1G_INGRESS_LATENCY;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
    }

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_GetEgressLatency(phy_handle_88q211x_t *dev, uint16_t *ns) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    switch (dev->speed) {

        case PHY_SPEED_100M:
            static_assert(PHY_88Q211X_100M_EGRESS_LATENCY < UINT16_MAX);
            *ns = PHY_88Q211X_100M_EGRESS_LATENCY;
            break;

        case PHY_SPEED_1G:
            static_assert(PHY_88Q211X_1G_EGRESS_LATENCY < UINT16_MAX);
            *ns = PHY_88Q211X_1G_EGRESS_LATENCY;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
    }

    PHY_UNLOCK;
    return status;
}
