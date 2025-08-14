/*
 * 88q211x_bist.h
 *
 *  Created on: Aug 14, 2025
 *      Author: bens1
 */

#ifndef INC_88Q211X_BIST_H_
#define INC_88Q211X_BIST_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "88q211x.h"


/* Mux A */
typedef enum {
    PHY_HOST_RX_88Q211X_LINE_RX    = 0x0,
    PHY_HOST_RX_88Q211X_HOST_TX    = 0x2,
    PHY_HOST_RX_88Q211X_PACKET_GEN = 0x3,
} phy_host_rx_88q211x_t;

/* Mux B */
typedef enum {
    PHY_LINE_TX_88Q211X_HOST_TX    = 0x0,
    PHY_LINE_TX_88Q211X_LINE_RX    = 0x2,
    PHY_LINE_TX_88Q211X_PACKET_GEN = 0x3,
} phy_line_tx_88q211x_t;

/* Mux C */
typedef enum {
    PHY_PACKET_CHECK_88Q211X_LINE       = 0x0,
    PHY_PACKET_CHECK_88Q211X_HOST       = 0x2,
    PHY_PACKET_CHECK_88Q211X_PACKET_GEN = 0x3,
} phy_packet_check_88q211x_t;


phy_status_t PHY_88Q211X_EnablePacketGenerator(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisablePacketGenerator(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_EnablePacketChecker(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisablePacketChecker(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_ReadPacketCheckerCounters(phy_handle_88q211x_t *dev, uint8_t *packets_received, uint8_t *errors, bool clear);

phy_status_t PHY_88Q211X_ConfigureGMIISteering(phy_handle_88q211x_t *dev, phy_host_rx_88q211x_t host_rx, phy_line_tx_88q211x_t line_tx, phy_packet_check_88q211x_t packet_check);
phy_status_t PHY_88Q211X_ResetGMIISteering(phy_handle_88q211x_t *dev);


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_BIST_H_ */