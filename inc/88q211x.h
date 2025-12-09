/*
 * 88q211x.h
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#ifndef INC_88Q211X_H_
#define INC_88Q211X_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdbool.h"
#include "stdatomic.h"

#include "phy_common.h"
#include "internal/phy_utils.h"


#define PHY_88Q211X_OUI          0x100158
#define PHY_88Q211X_MODEL_NUMBER 0x18


typedef enum {
    PHY_STATE_88Q211X_UNCONFIGURED = 0,
    PHY_STATE_88Q211X_IDLE,
    PHY_STATE_88Q211X_POWER_DOWN,
    PHY_STATE_88Q211X_LINK_UP,
} phy_state_88q211x_t;

typedef enum {
    PHY_CABLE_STATE_88Q211X_NOT_STARTED,
    PHY_CABLE_STATE_88Q211X_TERMINATED,
    PHY_CABLE_STATE_88Q211X_SHORT,
    PHY_CABLE_STATE_88Q211X_OPEN,
    PHY_CABLE_STATE_88Q211X_ERROR,
} phy_cable_state_88q211x_t;

typedef enum {
    PHY_FIFO_SIZE_88Q211X_10KB = 0x0,
    PHY_FIFO_SIZE_88Q211X_15KB = 0x1,
    PHY_FIFO_SIZE_88Q211X_20KB = 0x2,
    PHY_FIFO_SIZE_88Q211X_25KB = 0x3,
} phy_fifo_size_88q211x_t;

/* Stores information about driver events */
typedef struct {
    atomic_int_fast32_t writes;
    atomic_int_fast32_t reads;
    atomic_int_fast32_t smi_errors;
    atomic_int_fast32_t crc_errors;
    atomic_int_fast32_t tx_faults;
    atomic_int_fast32_t rx_faults;
} phy_event_counters_88q211x_t;

typedef struct {

    /* Common attributes */
    phy_variant_t   variant;
    uint8_t         phy_addr;
    phy_interface_t interface;
    phy_speed_t     default_speed; /* The speed of the PHY immediately after initialisation */
    phy_role_t      default_role;
    uint32_t        timeout;       /* Timeout in ms for doing anything with a timeout (read, write, take mutex etc) */

    /* PHY Specific attributes */
    bool                    tx_clk_internal_delay; /* RGMII Only */
    bool                    rx_clk_internal_delay; /* RGMII Only */
    phy_fifo_size_88q211x_t fifo_size;

} phy_config_88q211x_t;

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
    phy_config_88q211x_t         config;
    phy_event_counters_88q211x_t events;
    phy_state_88q211x_t          state;

} phy_handle_88q211x_t;


phy_status_t PHY_88Q211X_Init(phy_handle_88q211x_t *dev, const phy_config_88q211x_t *config, const phy_callbacks_t *callbacks, void *callback_context);
phy_status_t PHY_88Q211X_Deinit(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_EnableInterrupts(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableInterrupts(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_ProcessInterrupt(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_GetLinkState(phy_handle_88q211x_t *dev, bool *linkup);
phy_status_t PHY_88Q211X_GetLinkInfo(phy_handle_88q211x_t *dev, bool *linkup, bool *remote_receiver_ok, bool *local_receiver_ok, bool *polarity_done, bool *alignment_done, bool *descrambler_locked);
phy_status_t PHY_88Q211X_GetLinkDropCount(phy_handle_88q211x_t *dev, uint8_t links_dropped);

phy_status_t PHY_88Q211X_EnableTransmit(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableTransmit(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_EnableAutoNegotiation(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableAutoNegotiation(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_GetAutoNegotiationStatus(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_SetSpeed(phy_handle_88q211x_t *dev, phy_speed_t speed);
phy_status_t PHY_88Q211X_GetSpeed(phy_handle_88q211x_t *dev, phy_speed_t *speed);

phy_status_t PHY_88Q211X_GetDuplex(phy_handle_88q211x_t *dev, phy_duplex_t *duplex);

phy_status_t PHY_88Q211X_SetRole(phy_handle_88q211x_t *dev, phy_role_t role);
phy_status_t PHY_88Q211X_GetRole(phy_handle_88q211x_t *dev, phy_role_t *role);

phy_status_t PHY_88Q211X_EnableAutoPolarityCorrection(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableAutoPolarityCorrection(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_GetPolarity(phy_handle_88q211x_t *dev, bool *normal);

phy_status_t PHY_88Q211X_StartVCT(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_GetVCTResults(phy_handle_88q211x_t *dev, phy_cable_state_88q211x_t *cable_state, uint32_t *maximum_peak_distance);

phy_status_t PHY_88Q211X_CheckFaults(phy_handle_88q211x_t *dev, phy_fault_t *fault);

phy_status_t PHY_88Q211X_Start100MBIST(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_Stop100MBIST(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_Get100MBISTResults(phy_handle_88q211x_t *dev, bool *error);

phy_status_t PHY_88Q211X_Start1000MBIST(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_Stop1000MBIST(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_Get1000MBISTResults(phy_handle_88q211x_t *dev, bool *error);

phy_status_t PHY_88Q211X_RunRGMIIImpedanceCalibration(phy_handle_88q211x_t *dev);

phy_status_t PHY_88Q211X_EnableTemperatureSensor(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_ReadTemperature(phy_handle_88q211x_t *dev, float *temp, bool *valid);

phy_status_t PHY_88Q211X_EnableIEEEPowerDown(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableIEEEPowerDown(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_EnableLPSDPowerDown(phy_handle_88q211x_t *dev);
phy_status_t PHY_88Q211X_DisableLPSDPowerDown(phy_handle_88q211x_t *dev);

/* TODO: PTP functions */


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_H_ */
