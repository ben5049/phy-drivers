/*
 * phy_common.h
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#ifndef INC_PHY_COMMON_H_
#define INC_PHY_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "hal.h"


typedef enum {
    PHY_OK      = 0,
    PHY_ERROR   = 1,
    PHY_BUSY    = 2,
    PHY_TIMEOUT = 3,
    PHY_ALREADY_CONFIGURED,
    PHY_PARAMETER_ERROR,
    PHY_NOT_IMPLEMENTED_ERROR,
    PHY_ID_ERROR,
} phy_status_t;

typedef enum {
    PHY_VARIANT_88Q2111,
    PHY_VARIANT_88Q2112,
    PHY_VARIANT_LAN8670,
    PHY_VARIANT_LAN8671,
    PHY_VARIANT_LAN8672,
} phy_variant_t;

typedef enum {
    PHY_INTERFACE_UNKNOWN = 0x0,
    PHY_INTERFACE_MII     = 0x1,
    PHY_INTERFACE_RMII    = 0x2,
    PHY_INTERFACE_GMII    = 0x3,
    PHY_INTERFACE_RGMII   = 0x4,
    PHY_INTERFACE_SGMII   = 0x5,
    PHY_INTERFACE_INVALID = 0x6
} phy_interface_t;

typedef enum {
    PHY_SPEED_UNKNOWN = 0x0,
    PHY_SPEED_10M     = 0x1,
    PHY_SPEED_100M    = 0x2,
    PHY_SPEED_1G      = 0x3,
    PHY_SPEED_INVALID = 0x4
} phy_speed_t;

typedef enum {
    PHY_DUPLEX_UNKNOWN = 0x0,
    PHY_HALF_DUPLEX    = 0x1,
    PHY_FULL_DUPLEX    = 0x2,
    PHY_DUPLEX_INVALID = 0x3
} phy_duplex_t;

typedef enum {
    PHY_ROLE_UNKNOWN = 0x0,
    PHY_ROLE_SLAVE   = 0x1,
    PHY_ROLE_MASTER  = 0x2,
    PHY_ROLE_INVALID = 0x3
} phy_role_t;

typedef phy_status_t (*phy_callback_read_reg_t)(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t *data, uint32_t timeout, void *context);
typedef phy_status_t (*phy_callback_write_reg_t)(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t data, uint32_t timeout, void *context);
typedef uint32_t (*phy_callback_get_time_ms_t)(void *context);
typedef void (*phy_callback_delay_ms_t)(uint32_t ms, void *context);
typedef void (*phy_callback_delay_ns_t)(uint32_t ns, void *context);
typedef phy_status_t (*phy_callback_take_mutex_t)(uint32_t timeout, void *context);
typedef phy_status_t (*phy_callback_give_mutex_t)(void *context);

typedef struct {
    phy_callback_read_reg_t    callback_read_reg;    /* Read from a register */
    phy_callback_write_reg_t   callback_write_reg;   /* Write to a register */
    phy_callback_get_time_ms_t callback_get_time_ms; /* Get time in ms */
    phy_callback_delay_ms_t    callback_delay_ms;    /* Non-blocking delay in ms */
    phy_callback_delay_ns_t    callback_delay_ns;    /* Blocking delay in ns */
    phy_callback_take_mutex_t  callback_take_mutex;  /* Take the mutex protecting the device */
    phy_callback_give_mutex_t  callback_give_mutex;  /* Give the mutex protecting the device */
} phy_callbacks_t;

typedef struct {
    phy_variant_t   variant;
    uint8_t         phy_addr;
    phy_interface_t interface;
    uint32_t        timeout;
} phy_config_base_t;

typedef struct {
    phy_speed_t            speed;
    phy_duplex_t           duplex;
    bool                   autoneg;
    phy_role_t             role;
    const phy_callbacks_t *callbacks;
    void                  *callback_context;
} phy_handle_base_t;


phy_status_t PHY_Init(void *dev, void *config, phy_callbacks_t *callbacks, void *callback_context);

void PHY_GetDuplex(void *dev, phy_duplex_t *duplex);
void PHY_GetSpeed(void *dev, phy_speed_t *speed);


#ifdef __cplusplus
}
#endif

#endif /* INC_PHY_COMMON_H_ */
