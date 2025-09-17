/*
 * 88q211x_xmii.h
 *
 *  Created on: Sep 17, 2025
 *      Author: bens1
 */

#ifndef INC_88Q211X_XMII_H_
#define INC_88Q211X_XMII_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "88q211x.h"


phy_status_t PHY_88Q211X_SoftwareResetRGMII(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_ConfigureRGMII(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_SoftwareResetSGMII(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_ConfigureSGMII(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_PowerDownSGMII(phy_handle_88q211x_t *dev, bool enable);


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_XMII_H_ */