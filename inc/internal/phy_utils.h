/*
 * phy_utils.h
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#ifndef INC_PHY_UTILS_H_
#define INC_PHY_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stddef.h"
#include "assert.h"

#include "phy_common.h"


#define PHY_CHECK_RET(status)                \
    do {                                     \
        if (status != PHY_OK) return status; \
    } while (0)

#define PHY_CHECK_END(status)           \
    do {                                \
        if (status != PHY_OK) goto end; \
    } while (0)

#define PHY_LOCK                                                                                  \
    do {                                                                                          \
        status = dev->callbacks->callback_take_mutex(dev->config.timeout, dev->callback_context); \
        PHY_CHECK_RET(status);                                                                    \
    } while (0)

#define PHY_UNLOCK dev->callbacks->callback_give_mutex(dev->callback_context)

#define PHY_CHECK_MEMBER_OFFSET(instance, base, member)                 \
    static_assert(offsetof(instance, member) == offsetof(base, member), \
                  #member " offset mismatch");

#define PHY_CHECK_MEMBER_SIZE(instance, base, member)                               \
    static_assert(sizeof(((instance *) 0)->member) == sizeof(((base *) 0)->member), \
                  #member " size mismatch");

#define PHY_CHECK_MEMBER_COMPATIBILITY(instance, base, member) \
    PHY_CHECK_MEMBER_OFFSET(instance, base, member);           \
    PHY_CHECK_MEMBER_SIZE(instance, base, member);

#define PHY_CHECK_HANDLE_MEMBERS(phy_handle_type)                                            \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, speed);               \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, duplex);              \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, autoneg);             \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, role);                \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, linkup);              \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, temp_sensor_enabled); \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, callbacks);           \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_handle_type, phy_handle_base_t, callback_context);    \
    PHY_CHECK_MEMBER_OFFSET(phy_handle_type, phy_handle_base_t, config);

#define PHY_CHECK_CONFIG_MEMBERS(phy_config_type)                                      \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, variant);       \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, phy_addr);      \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, interface);     \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, default_speed); \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, default_role);  \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, c45_en);        \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_config_type, phy_config_base_t, timeout);

#define PHY_CHECK_EVENTS_MEMBERS(phy_events_type)                                          \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_events_type, phy_event_counters_base_t, writes)     \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_events_type, phy_event_counters_base_t, reads)      \
    PHY_CHECK_MEMBER_COMPATIBILITY(phy_events_type, phy_event_counters_base_t, smi_errors)


#if PHY_LOGGING_ENABLED == 1
#define PHY_LOG(format, ...) dev->callbacks->callback_write_log("%s:%u (addr=%u) " format, __FILE_NAME__, __LINE__, dev->config.phy_addr, ##__VA_ARGS__)
#else
#define PHY_LOG(format, ...) PHY_OK
#endif


#ifdef __cplusplus
}
#endif

#endif /* INC_PHY_UTILS_H_ */
