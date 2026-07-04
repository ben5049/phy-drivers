/*
 * dp83867_ptp.c
 *
 *  Created on: Jul 4, 2026
 *      Author: bens1
 */

#include "dp83867.h"
#include "dp83867_ptp.h"


phy_status_t PHY_DP83867_GetIngressLatency(phy_handle_dp83867_t *dev, uint16_t *ns) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    switch (dev->config.interface) {

        case PHY_INTERFACE_RGMII:
            static_assert(PHY_DP83867_1G_RGMII_RX_LATENCY < UINT16_MAX);
            *ns = PHY_DP83867_1G_RGMII_RX_LATENCY;
            break;

        case PHY_INTERFACE_MII: /* No better source, fallthrough */
        case PHY_INTERFACE_GMII:
            static_assert(PHY_DP83867_1G_GMII_RX_LATENCY < UINT16_MAX);
            *ns = PHY_DP83867_1G_GMII_RX_LATENCY;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
    }

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_DP83867_GetEgressLatency(phy_handle_dp83867_t *dev, uint16_t *ns) {

    phy_status_t status = PHY_OK;

    PHY_LOCK;

    switch (dev->config.interface) {

        case PHY_INTERFACE_RGMII:
            static_assert(PHY_DP83867_1G_RGMII_TX_LATENCY < UINT16_MAX);
            *ns = PHY_DP83867_1G_RGMII_TX_LATENCY;
            break;

        case PHY_INTERFACE_MII: /* No better source, fallthrough */
        case PHY_INTERFACE_GMII:
            static_assert(PHY_DP83867_1G_GMII_TX_LATENCY < UINT16_MAX);
            *ns = PHY_DP83867_1G_GMII_TX_LATENCY;
            break;

        default:
            status = PHY_PARAMETER_ERROR;
    }

    PHY_UNLOCK;
    return status;
}
