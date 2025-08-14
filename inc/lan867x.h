/*
 * lan867x.h
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#ifndef INC_LAN867X_H_
#define INC_LAN867X_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdbool.h"

#include "phy_common.h"


typedef struct {
    phy_variant_t   variant;
    uint8_t         phy_addr;
    phy_interface_t interface;
    uint32_t        timeout;
} phy_config_lan867x_t;


typedef struct {
    phy_speed_t            speed;
    phy_duplex_t           duplex;
    bool                   autoneg;
    phy_role_t             role;
    const phy_callbacks_t *callbacks;
    void                  *callback_context;

    phy_config_lan867x_t *config;
} phy_handle_lan867x_t;


phy_status_t PHY_LAN867X_Init(phy_handle_lan867x_t *dev, const phy_config_lan867x_t *config, const phy_callbacks_t *callbacks, void *callback_context);


#ifdef __cplusplus
}
#endif

#endif /* INC_LAN867X_H_ */
