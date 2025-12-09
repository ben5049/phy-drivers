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


/* Stores information about driver events */
typedef struct {
    atomic_int_fast32_t writes;
    atomic_int_fast32_t reads;
    atomic_int_fast32_t smi_errors;
} phy_event_counters_lan867x_t;

/* Silicon revision of the chip */
typedef enum {
    PHY_LAN867X_SI_REV_C2 = 0b0101,
    PHY_LAN867X_SI_REV_D0 = 0b0110
} phy_si_rev_lan867x_t;

typedef struct {

    /* Common attributes */
    phy_variant_t   variant;
    uint8_t         phy_addr;
    phy_interface_t interface;
    phy_speed_t     default_speed; /* The speed of the PHY immediately after initialisation */
    phy_role_t      default_role;
    uint32_t        timeout;

    /* PHY Specific attributes */
    bool    plca_enabled; /* Should usually be enabled */
    uint8_t plca_id;      /* 0 = PLCA coordinator, 1 to 0xfe = PLCA follower */
    uint8_t plca_nodes;   /* The number of PLCA transmit opportunities. Only valid if plca_id = 0 */

} phy_config_lan867x_t;

typedef struct {

    /* Common attributes */
    phy_speed_t            speed;
    phy_duplex_t           duplex;
    bool                   autoneg;
    phy_role_t             role;
    bool                   linkup;
    bool                   temp_sensor_enabled;
    const phy_callbacks_t *callbacks;
    void                  *callback_context;

    /* PHY Specific attributes */
    phy_config_lan867x_t         config;
    phy_event_counters_lan867x_t events;
    phy_si_rev_lan867x_t         silicon_revision;

} phy_handle_lan867x_t;


phy_status_t PHY_LAN867X_Init(phy_handle_lan867x_t *dev, const phy_config_lan867x_t *config, const phy_callbacks_t *callbacks, void *callback_context);


#ifdef __cplusplus
}
#endif

#endif /* INC_LAN867X_H_ */
