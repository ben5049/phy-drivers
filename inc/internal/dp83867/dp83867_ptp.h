/*
 * dp83867_ptp.h
 *
 *  Created on: Jul 4, 2026
 *      Author: bens1
 */

#ifndef INC_DP83867_PTP_H_
#define INC_DP83867_PTP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "dp83867.h"


/* MII Latency and other speed latencies are not given in the datasheet */
#define PHY_DP83867_1G_RGMII_RX_LATENCY (288) /* ns */
#define PHY_DP83867_1G_GMII_RX_LATENCY  (264) /* ns */
#define PHY_DP83867_1G_RGMII_TX_LATENCY (88)  /* ns */
#define PHY_DP83867_1G_GMII_TX_LATENCY  (72)  /* ns */


#ifdef __cplusplus
}
#endif

#endif /* INC_DP83867_PTP_H_ */