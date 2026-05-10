/*
 * dp83867_regs.h
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#ifndef INC_DP83867_REGS_H_
#define INC_DP83867_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "phy_io.h"

#include "dp83867.h"


/* What to put in the MMD address field */
#define PHY_DP83867_MMD_BASIC    (0x00)
#define PHY_DP83867_MMD_EXTENDED (0x1f)


static inline phy_status_t phy_dp83867_write_reg(phy_handle_dp83867_t *dev, uint16_t addr, uint16_t data) {
    if (addr <= 0x1f) {
        return PHY_WRITE_REG(dev, PHY_DP83867_MMD_BASIC, addr, data);
    } else {
        return PHY_WRITE_REG(dev, PHY_DP83867_MMD_EXTENDED, addr, data);
    }
}

static inline phy_status_t phy_dp83867_read_reg(phy_handle_dp83867_t *dev, uint16_t addr, uint16_t *data) {
    if (addr <= 0x1f) {
        return PHY_READ_REG(dev, PHY_DP83867_MMD_BASIC, addr, data);
    } else {
        return PHY_READ_REG(dev, PHY_DP83867_MMD_EXTENDED, addr, data);
    }
}


/* ---------------------------------------------------------------------------- */
/* Basic Registers */
/* ---------------------------------------------------------------------------- */

enum phy_dp83867_basic_reg_e {
    PHY_DP83867_REG_BASIC_BMCR    = 0x00,
    PHY_DP83867_REG_BASIC_BMSR    = 0x01,
    PHY_DP83867_REG_BASIC_PHYIDR1 = 0x02,
    PHY_DP83867_REG_BASIC_PHYIDR2 = 0x03,
    PHY_DP83867_REG_BASIC_ANAR    = 0x04,
    PHY_DP83867_REG_BASIC_ANLPAR  = 0x05,
    PHY_DP83867_REG_BASIC_ANER    = 0x06,
    PHY_DP83867_REG_BASIC_ANNPTR  = 0x07,
    PHY_DP83867_REG_BASIC_ANNPRR  = 0x08,
    PHY_DP83867_REG_BASIC_CFG1    = 0x09,
    PHY_DP83867_REG_BASIC_STS1    = 0x0a,
    PHY_DP83867_REG_BASIC_REGCR   = 0x0d,
    PHY_DP83867_REG_BASIC_ADDAR   = 0x0e,
    PHY_DP83867_REG_BASIC_A1KSCR  = 0x0f,
    PHY_DP83867_REG_BASIC_PHYCR   = 0x10,
    PHY_DP83867_REG_BASIC_PHYSTS  = 0x11,
    PHY_DP83867_REG_BASIC_MICR    = 0x12,
    PHY_DP83867_REG_BASIC_ISR     = 0x13,
    PHY_DP83867_REG_BASIC_CFR2    = 0x14,
    PHY_DP83867_REG_BASIC_RECR    = 0x15,
    PHY_DP83867_REG_BASIC_BISCR   = 0x16,
    PHY_DP83867_REG_BASIC_STS2    = 0x17,
    PHY_DP83867_REG_BASIC_LEDCR1  = 0x18,
    PHY_DP83867_REG_BASIC_LEDCR2  = 0x19,
    PHY_DP83867_REG_BASIC_LEDCR3  = 0x1a,
    PHY_DP83867_REG_BASIC_CFG3    = 0x1e,
    PHY_DP83867_REG_BASIC_CTRL    = 0x1f,
};

/* BMCR */
#define PHY_DP83867_RESET           (1 << 15)
#define PHY_DP83867_LOOPBACK        (1 << 14)
#define PHY_DP83867_SPEED_LSB       (1 << 13)
#define PHY_DP83867_AUTONEG_EN      (1 << 12)
#define PHY_DP83867_POWER_DOWN      (1 << 11)
#define PHY_DP83867_ISOLATE         (1 << 10)
#define PHY_DP83867_AUTONEG_RESTART (1 << 9)
#define PHY_DP83867_BMCR_DUPLEX     (1 << 8)
#define PHY_DP83867_COL_TEST        (1 << 7)
#define PHY_DP83867_SPEED_MSB       (1 << 6)

/* BMSR */
#define PHY_DP83867_100BASE_T4          (1 << 15)
#define PHY_DP83867_100BASE_TX_FD       (1 << 14)
#define PHY_DP83867_100BASE_TX_HD       (1 << 13)
#define PHY_DP83867_10BASE_TE_FD        (1 << 12)
#define PHY_DP83867_10BASE_TE_HD        (1 << 11)
#define PHY_DP83867_100BASE_T2_FD       (1 << 10)
#define PHY_DP83867_100BASE_T2_HD       (1 << 9)
#define PHY_DP83867_EXTENDED_STATUS     (1 << 8)
#define PHY_DP83867_PSE                 (1 << 6) /* Preamble suppression supported, 1 by default */
#define PHY_DP83867_AUTONEG_DONE        (1 << 5)
#define PHY_DP83867_REMOTE_FAULT        (1 << 4)
#define PHY_DP83867_AUTONEG_ABILITY     (1 << 3)
#define PHY_DP83867_LINK_STATUS         (1 << 2)
#define PHY_DP83867_JABBER_DETECT       (1 << 1)
#define PHY_DP83867_EXTENDED_CAPABILITY (1 << 0)

/* PHYIDR */
#define PHY_DP83867_OUI_3_18_SHIFT        (0)
#define PHY_DP83867_OUI_3_18_MASK         (0xffff << PHY_DP83867_OUI_3_18_SHIFT)
#define PHY_DP83867_OUI_19_24_SHIFT       (10)
#define PHY_DP83867_OUI_19_24_MASK        (0x003f << PHY_DP83867_OUI_19_24_SHIFT)
#define PHY_DP83867_MODEL_NUMBER_SHIFT    (4)
#define PHY_DP83867_MODEL_NUMBER_MASK     (0x003f << PHY_DP83867_MODEL_NUMBER_SHIFT)
#define PHY_DP83867_REVISION_NUMBER_SHIFT (0)
#define PHY_DP83867_REVISION_NUMBER_MASK  (0x000f << PHY_DP83867_REVISION_NUMBER_SHIFT)

/* PHYCR */
#define PHY_DP83867_DISABLE_CLK125 (1 << 4)

/* PHYSTS */
#define PHY_DP83867_SPEED_SHIFT    (14)
#define PHY_DP83867_SPEED_MASK     (0x3 << PHY_DP83867_SPEED_SHIFT)
#define PHY_DP83867_SPEED_10MBPS   (0x00)
#define PHY_DP83867_SPEED_100MBPS  (0x01)
#define PHY_DP83867_SPEED_1000MBPS (0x02)
#define PHY_DP83867_PHYSTS_DUPLEX  (1 << 13)

/* ---------------------------------------------------------------------------- */
/* Extended Registers */
/* ---------------------------------------------------------------------------- */

enum phy_dp83867_extended_reg_e {

    PHY_DP83867_REG_EXT_MSE_CHANNEL_A = 0x0225,
    PHY_DP83867_REG_EXT_MSE_CHANNEL_B = 0x0265,
    PHY_DP83867_REG_EXT_MSE_CHANNEL_C = 0x02a5,
    PHY_DP83867_REG_EXT_MSE_CHANNEL_D = 0x02e5,

    // TODO: add more
};


#ifdef __cplusplus
}
#endif

#endif /* INC_DP83867_REGS_H_ */
