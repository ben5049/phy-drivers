/*
 * phy_io.h
 *
 *  Created on: Jan 1, 2026
 *      Author: bens1
 *
 */

#ifndef INC_PHY_IO_H_
#define INC_PHY_IO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "phy_common.h"


/* MMD Device addresses */
#define PHY_MMD_VENDOR_SPECIFIC (0x1f)

/* Registers used for clause 22 access of clause 45 register space */
#define PHY_REGCR                  (0x0d)
#define PHY_ADDAR                  (0x0e)

#define PHY_REGCR_FNCTN_SHIFT      (14)
#define PHY_REGCR_FNCTN_MASK       (0x3 << PHY_LAN867X_MMDCTRL_FNCTN_SHIFT)
#define PHY_REGCR_FNCTN_ADDR       (0) /* Address */
#define PHY_REGCR_FNCTN_DATA       (1) /* Data - No post increment */
#define PHY_REGCR_FNCTN_DATA_PI_RW (2) /* Data - Post increment on reads and writes */
#define PHY_REGCR_FNCTN_DATA_PI_W  (3) /* Data - Post increment on writes only */
#define PHY_REGCR_DEVAD_SHIFT      (0)
#define PHY_REGCR_DEVAD_MASK       (0x1f << PHY_LAN867X_MMDCTRL_DEVAD_SHIFT)


#define PHY_READ_REG(dev, mmd_addr, reg_addr, data)                                   \
    ({                                                                                \
        phy_status_t __status;                                                        \
        __status = phy_read_reg((phy_handle_base_t *) dev, mmd_addr, reg_addr, data); \
        if (__status != PHY_OK) {                                                     \
            dev->events.smi_errors++;                                                 \
        } else {                                                                      \
            dev->events.reads++;                                                      \
        }                                                                             \
        __status;                                                                     \
    })

#define PHY_WRITE_REG(dev, mmd_addr, reg_addr, data)                                   \
    ({                                                                                 \
        phy_status_t __status;                                                         \
        __status = phy_write_reg((phy_handle_base_t *) dev, mmd_addr, reg_addr, data); \
        if (__status != PHY_OK) {                                                      \
            dev->events.smi_errors++;                                                  \
        } else {                                                                       \
            dev->events.writes++;                                                      \
        }                                                                              \
        __status;                                                                      \
    })


phy_status_t phy_read_reg(phy_handle_base_t *dev, uint8_t mmd_addr, uint16_t reg_addr, uint16_t *data);
phy_status_t phy_write_reg(phy_handle_base_t *dev, uint8_t mmd_addr, uint16_t reg_addr, uint16_t data);


#ifdef __cplusplus
}
#endif

#endif /* INC_PHY_IO_H_ */
