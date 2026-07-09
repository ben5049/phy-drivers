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


/* No official Marvell or Infineon sources could be found so the following
 * sources were used in addition to finding them experimentally:
 * - DP83TC815-Q1:   100BASE-T1 https://www.ti.com/lit/gpn/DP83TC815-Q1
 * - DP83TG720R-Q1: 1000BASE-T1 https://www.ti.com/lit/gpn/DP83TG720R-Q1
 */
#define PHY_88Q211X_100M_INGRESS_LATENCY (960)  /* ns (estimated) */
#define PHY_88Q211X_1G_INGRESS_LATENCY   (4400) /* ns (estimated), larger than others due to FEC */
#define PHY_88Q211X_100M_EGRESS_LATENCY  (480)  /* ns (estimated) */
#define PHY_88Q211X_1G_EGRESS_LATENCY    (480)  /* ns (estimated) */


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_PTP_H_ */