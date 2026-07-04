/*
 * 88q211x_ptp.h
 *
 *  Created on: Jul 4, 2026
 *      Author: bens1
 */

#ifndef INC_88Q211X_PTP_H_
#define INC_88Q211X_PTP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "88q211x.h"


/* Latencies from https://intrepidcs.com/products/automotive-ethernet-tools/rad-gigastar-1000base-t1-active-tap-gateway-vehicle-interface/rad-gigastar-specifications/
 * Since no official Marvell or Infineon sources could be found */
#define PHY_88Q211X_100M_INGRESS_LATENCY (780) /* ns. TODO: Check this is the same as 1G */
#define PHY_88Q211X_1G_INGRESS_LATENCY   (780) /* ns */
#define PHY_88Q211X_100M_EGRESS_LATENCY  (240) /* ns. TODO: Check this is the same as 1G */
#define PHY_88Q211X_1G_EGRESS_LATENCY    (240) /* ns */


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_PTP_H_ */