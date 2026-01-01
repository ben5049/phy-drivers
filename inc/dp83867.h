/*
 * dp83867.h
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#ifndef INC_DP83867_H_
#define INC_DP83867_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdbool.h"
#include "stdatomic.h"

#include "phy_common.h"
#include "internal/phy_utils.h"


#define PHY_DP83867_OUI 0x0


/* Stores information about driver events */
typedef struct {

    /* Common attributes */
    atomic_int_fast32_t writes;
    atomic_int_fast32_t reads;
    atomic_int_fast32_t smi_errors;

    /* PHY Specific attributes */


} phy_event_counters_dp83867_t;

typedef struct {

    /* Common attributes */
    phy_variant_t   variant;
    uint8_t         phy_addr;
    phy_interface_t interface;
    phy_speed_t     default_speed; /* The speed of the PHY immediately after initialisation */
    phy_role_t      default_role;
    bool            c45_en;        /* Whether or not clause 45 access mechanism has been implemented */
    uint32_t        timeout;       /* Timeout in ms for doing anything with a timeout (read, write, take mutex etc) */

    /* PHY Specific attributes */


} phy_config_dp83867_t;

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
    phy_config_dp83867_t         config;
    phy_event_counters_dp83867_t events;

} phy_handle_dp83867_t;


phy_status_t PHY_DP83867_Init(phy_handle_dp83867_t *dev, const phy_config_dp83867_t *config, const phy_callbacks_t *callbacks, void *callback_context);
phy_status_t PHY_DP83867_Deinit(phy_handle_dp83867_t *dev);

phy_status_t PHY_DP83867_EnableInterrupts(phy_handle_dp83867_t *dev);
phy_status_t PHY_DP83867_DisableInterrupts(phy_handle_dp83867_t *dev);
phy_status_t PHY_DP83867_ProcessInterrupt(phy_handle_dp83867_t *dev);

phy_status_t PHY_DP83867_GetLinkState(phy_handle_dp83867_t *dev, bool *linkup);

phy_status_t PHY_DP83867_EnableAutoNegotiation(phy_handle_dp83867_t *dev);
phy_status_t PHY_DP83867_DisableAutoNegotiation(phy_handle_dp83867_t *dev);
phy_status_t PHY_DP83867_GetAutoNegotiationStatus(phy_handle_dp83867_t *dev);

phy_status_t PHY_DP83867_SetSpeed(phy_handle_dp83867_t *dev, phy_speed_t speed);
phy_status_t PHY_DP83867_GetSpeed(phy_handle_dp83867_t *dev, phy_speed_t *speed);

phy_status_t PHY_DP83867_GetDuplex(phy_handle_dp83867_t *dev, phy_duplex_t *duplex);

phy_status_t PHY_DP83867_GetSQI(phy_handle_dp83867_t *dev, uint8_t *sqi);

phy_status_t PHY_DP83867_EnableTemperatureSensor(phy_handle_dp83867_t *dev);
phy_status_t PHY_DP83867_ReadTemperature(phy_handle_dp83867_t *dev, float *temp, bool *valid);


#ifdef __cplusplus
}
#endif

#endif /* INC_DP83867_H_ */
