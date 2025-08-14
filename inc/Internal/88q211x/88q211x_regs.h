/*
 * 88q211x_regs.h
 *
 *  Created on: Aug 12, 2025
 *      Author: bens1
 */

#ifndef INC_88Q211X_REGS_H_
#define INC_88Q211X_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "88q211x.h"


/* ---------------------------------------------------------------------------- */
/* IEEE PMA/PMD Registers */
/* ---------------------------------------------------------------------------- */

enum phy_88q211x_pma_pmd_reg_e {
    PHY_88Q211X_REG_PMA_PMD_CTRL_1                    = 0x0000,
    PHY_88Q211X_REG_PMA_PMD_DEV_ID_1                  = 0x0002,
    PHY_88Q211X_REG_PMA_PMD_DEV_ID_2                  = 0x0003,
    PHY_88Q211X_REG_PMA_PMD_SPEED_ABILITY             = 0x0004,
    PHY_88Q211X_REG_PMA_PMD_PKG_DEVS_1                = 0x0005,
    PHY_88Q211X_REG_PMA_PMD_PKG_DEVS_2                = 0x0006,
    PHY_88Q211X_REG_PMA_PMD_CTRL_2                    = 0x0007,
    PHY_88Q211X_REG_10G_PMA_PMD_STATUS_2              = 0x0008,
    PHY_88Q211X_REG_10G_PMA_TX_DISABLE                = 0x0009,
    PHY_88Q211X_REG_10G_PMA_PMD_SIG_DET               = 0x000a,
    PHY_88Q211X_REG_PMA_PMD_EXT_ABILITY               = 0x000b,
    PHY_88Q211X_REG_PMA_PMD_PKG_ID_1                  = 0x000e,
    PHY_88Q211X_REG_PMA_PMD_PKG_ID_2                  = 0x000f,
    PHY_88Q211X_REG_BASE_T1_PMA_PMD_EXT_ABILITY       = 0x0012,
    PHY_88Q211X_REG_100BASE_T1_PMA_PMD_TEST_CTRL      = 0x0834,
    PHY_88Q211X_REG_BASE_T1_CTRL                      = 0x0900,
    PHY_88Q211X_REG_1000BASE_T1_PMA_STATUS            = 0x0901,
    PHY_88Q211X_REG_1000BASE_T1_TRAINING              = 0x0902,
    PHY_88Q211X_REG_1000BASE_T1_LINK_PARTNER_TRAINING = 0x0903,
    PHY_88Q211X_REG_1000BASE_T1_TEST_MODE_CTRL        = 0x0904,
};

enum phy_88q211x_pma_pmd_dev_e {
    PHY_88Q211X_DEV_PMA_PMD_CTRL_1                    = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_DEV_ID_1                  = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_DEV_ID_2                  = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_SPEED_ABILITY             = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_PKG_DEVS_1                = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_PKG_DEVS_2                = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_CTRL_2                    = 0x01,
    PHY_88Q211X_DEV_10G_PMA_PMD_STATUS_2              = 0x01,
    PHY_88Q211X_DEV_10G_PMA_TX_DISABLE                = 0x01,
    PHY_88Q211X_DEV_10G_PMA_PMD_SIG_DET               = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_EXT_ABILITY               = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_PKG_ID_1                  = 0x01,
    PHY_88Q211X_DEV_PMA_PMD_PKG_ID_2                  = 0x01,
    PHY_88Q211X_DEV_BASE_T1_PMA_PMD_EXT_ABILITY       = 0x01,
    PHY_88Q211X_DEV_100BASE_T1_PMA_PMD_TEST_CTRL      = 0x01,
    PHY_88Q211X_DEV_BASE_T1_CTRL                      = 0x01,
    PHY_88Q211X_DEV_1000BASE_T1_PMA_STATUS            = 0x01,
    PHY_88Q211X_DEV_1000BASE_T1_TRAINING              = 0x01,
    PHY_88Q211X_DEV_1000BASE_T1_LINK_PARTNER_TRAINING = 0x01,
    PHY_88Q211X_DEV_1000BASE_T1_TEST_MODE_CTRL        = 0x01,
};


#define PHY_88Q211X_OUI_3_18_SHIFT        (0)
#define PHY_88Q211X_OUI_3_18_MASK         (0xffff << PHY_88Q211X_OUI_3_18_SHIFT)
#define PHY_88Q211X_OUI_19_24_SHIFT       (10)
#define PHY_88Q211X_OUI_19_24_MASK        (0x3f << PHY_88Q211X_OUI_19_24_SHIFT)
#define PHY_88Q211X_MODEL_NUMBER_SHIFT    (4)
#define PHY_88Q211X_MODEL_NUMBER_MASK     (0x3f << PHY_88Q211X_MODEL_NUMBER_SHIFT)
#define PHY_88Q211X_REVISION_NUMBER_SHIFT (0)
#define PHY_88Q211X_REVISION_NUMBER_MASK  (0xf << PHY_88Q211X_REVISION_NUMBER_SHIFT)

/* ---------------------------------------------------------------------------- */
/* Common Control Registers */
/* ---------------------------------------------------------------------------- */

enum phy_88q211x_ctrl_reg_e {
    PHY_88Q211X_REG_RST_CTRL          = 0x8000,
    PHY_88Q211X_REG_TX_DISABLE_STATUS = 0x8002,
    PHY_88Q211X_REG_SYNCE_CTRL        = 0x8004,
    PHY_88Q211X_REG_INT_EN_1          = 0x8010,
    PHY_88Q211X_REG_GPIO_INT_STATUS   = 0x8011,
    PHY_88Q211X_REG_TEMP_2            = 0x8041,
    PHY_88Q211X_REG_TEMP_3            = 0x8042,
    PHY_88Q211X_REG_TEMP_4            = 0x8043,
    PHY_88Q211X_REG_INT_EN_2          = 0xfe16,
    PHY_88Q211X_REG_INT_STATUS        = 0xfe17,
    PHY_88Q211X_REG_TDR_CTRL          = 0xfec3,
    PHY_88Q211X_REG_TDR_STATUS_1      = 0xfedb,
    PHY_88Q211X_REG_TDR_STATUS_2      = 0xdedc,
    PHY_88Q211X_REG_IO_VOLTAGE_CTRL   = 0x8214,
    PHY_88Q211X_REG_LPSD_1            = 0x801c,
    PHY_88Q211X_REG_LPSD_2            = 0x801d,
};

enum phy_88q211x_ctrl_dev_e {
    PHY_88Q211X_DEV_RST_CTRL          = 0x03,
    PHY_88Q211X_DEV_TX_DISABLE_STATUS = 0x03,
    PHY_88Q211X_DEV_SYNCE_CTRL        = 0x03,
    PHY_88Q211X_DEV_INT_EN_1          = 0x03,
    PHY_88Q211X_DEV_GPIO_INT_STATUS   = 0x03,
    PHY_88Q211X_DEV_TEMP_2            = 0x03,
    PHY_88Q211X_DEV_TEMP_3            = 0x03,
    PHY_88Q211X_DEV_TEMP_4            = 0x03,
    PHY_88Q211X_DEV_INT_EN_2          = 0x03,
    PHY_88Q211X_DEV_INT_STATUS        = 0x03,
    PHY_88Q211X_DEV_TDR_CTRL          = 0x03,
    PHY_88Q211X_DEV_TDR_STATUS_1      = 0x03,
    PHY_88Q211X_DEV_TDR_STATUS_2      = 0x03,
    PHY_88Q211X_DEV_IO_VOLTAGE_CTRL   = 0x04,
    PHY_88Q211X_DEV_LPSD_1            = 0x03,
    PHY_88Q211X_DEV_LPSD_2            = 0x03,
};

#define PHY_88Q211X_RST_RGMII         (1 << 15)


#define PHY_88Q211X_TEMPERATURE_SHIFT (0)
#define PHY_88Q211X_TEMPERATURE_MASK  (0xff << PHY_88Q211X_TEMPERATURE_SHIFT)

/* ---------------------------------------------------------------------------- */
/* IEEE PCS Registers (Device 3) */
/* ---------------------------------------------------------------------------- */

// enum PHY_88Q211X_PCSRegs_Enum {
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
//     PHY_88Q211X_REG_ = 0x,
// };

/* ---------------------------------------------------------------------------- */
/* IEEE Auto-Negotiation Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* 100BASE-T1 Copper Unit Advance PCS Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* 1000BASE-T1 Copper Unit Advance PCS Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* Copper Unit Advance Auto-Negotiation Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* RGMII Registers */
/* ---------------------------------------------------------------------------- */


enum phy_88q211x_rgmii_reg_e {
    PHY_88Q211X_REG_RGMII_IMPEDANCE = 0x8000,
    PHY_88Q211X_REG_RGMII_COM_PORT  = 0x8001,
};

enum phy_88q211x_rgmii_dev_e {
    PHY_88Q211X_DEV_RGMII_IMPEDANCE = 0x1f,
    PHY_88Q211X_DEV_RGMII_COM_PORT  = 0x1f,
};


#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_RESTART             (1 << 15)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_COMPLETE            (1 << 14)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_ENABLE          (1 << 3)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_SHIFT     (0)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_MASK      (0x7 << PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_SHIFT)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_78_8_OHMS (0x0)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_64_5_OHMS (0x1)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_54_6_OHMS (0x2)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_47_3_OHMS (0x3)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_41_7_OHMS (0x4)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_37_3_OHMS (0x5)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_33_8_OHMS (0x6)
#define PHY_88Q211X_RGMII_IMPEDANCE_CALIBRATION_ODR_VALUE_30_9_OHMS (0x7)

#define PHY_88Q211X_RGMII_TX_CLK_INTERNAL_DELAY                     (1 << 15)
#define PHY_88Q211X_RGMII_RX_CLK_INTERNAL_DELAY                     (1 << 14)


/* ---------------------------------------------------------------------------- */
/* SGMII Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* PTP Registers */
/* ---------------------------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* INC_88Q211X_REGS_H_ */