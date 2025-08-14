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
    PHY_88Q211X_REG_TDR_STATUS_2      = 0xfedc,
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

#define PHY_88Q211X_RST_RGMII                      (1 << 15)

#define PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT    (14)
#define PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK     (0x3 << PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT)
#define PHY_88Q211X_TEMPERATURE_SENSOR_1HZ         (0x0)
#define PHY_88Q211X_TEMPERATURE_SENSOR_CUSTOM_RATE (0x1)
#define PHY_88Q211X_TEMPERATURE_SENSOR_ONE_SHOT    (0x2)
#define PHY_88Q211X_TEMPERATURE_SENSOR_DISABLE     (0x3)

#define PHY_88Q211X_TEMPERATURE_SHIFT              (0)
#define PHY_88Q211X_TEMPERATURE_MASK               (0xff << PHY_88Q211X_TEMPERATURE_SHIFT)

#define PHY_88Q211X_TDR_EN                         (1 << 14)

#define PHY_88Q211X_TDR_TEST_COMPLETE              (1 << 10)
#define PHY_88Q211X_TDR_VCT_DISTANCE_SHIFT         (0)
#define PHY_88Q211X_TDR_VCT_DISTANCE_MASK          (0x03ff << PHY_88Q211X_TDR_VCT_DISTANCE_SHIFT)

#define PHY_88Q211X_TDR_VCT_POLARITY               (1 << 7)
#define PHY_88Q211X_TDR_VCT_AMPLITUDE_SHIFT        (0)
#define PHY_88Q211X_TDR_VCT_AMPLITUDE_MASK         (0x007f << PHY_88Q211X_TDR_VCT_AMPLITUDE_SHIFT)

/* ---------------------------------------------------------------------------- */
/* IEEE PCS Registers (Device 3) */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* IEEE Auto-Negotiation Registers */
/* ---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/* 3.5 100BASE-T1 Copper Unit Advance PCS Registers */
/* ---------------------------------------------------------------------------- */

enum phy_88q211x_100base_t1_pcs_reg_e {
    PHY_88Q211X_REG_100BASE_T1_CU_CTRL       = 0x8100,
    PHY_88Q211X_REG_100BASE_T1_STATUS_1      = 0x8108,
    PHY_88Q211X_REG_100BASE_T1_STATUS_2      = 0x8109,
    PHY_88Q211X_REG_100BASE_T1_INT_EN_1      = 0x8112,
    PHY_88Q211X_REG_100BASE_T1_CU_INT_STATUS = 0x8113,
    PHY_88Q211X_REG_100BASE_T1_INT_STATUS_1  = 0x8117,
    PHY_88Q211X_REG_LINK_DROP_COUNTER        = 0x8120,
    PHY_88Q211X_REG_MAC_CTRL                 = 0x8210,
    PHY_88Q211X_REG_MAC_INT_EN               = 0x8212,
    PHY_88Q211X_REG_MAC_STATUS               = 0x8213,
    PHY_88Q211X_REG_TX_FIFO_OVERFLOW_COUNTER = 0x8214,
    PHY_88Q211X_REG_COUNTER_CTRL             = 0x8220,
    PHY_88Q211X_REG_BAD_LINK_COUNTER         = 0x8221,
    PHY_88Q211X_REG_BAD_SSD_COUNTER          = 0x8222,
    PHY_88Q211X_REG_BAD_ESD_COUNTER          = 0x8223,
    PHY_88Q211X_REG_RX_ERROR_COUNTER         = 0x8224,
    PHY_88Q211X_REG_RECEIVER_STATUS          = 0x8230,
    PHY_88Q211X_REG_100BASE_T1_INT_EN_2      = 0x8300,
    PHY_88Q211X_REG_100BASE_T1_INT_STATUS_2  = 0x8301,
    PHY_88Q211X_REG_GPIO_TX_ENABLE_CTRL_1    = 0x8302,
    PHY_88Q211X_REG_GPIO_TX_ENABLE_CTRL_2    = 0x8303,
    PHY_88Q211X_REG_GPIO_TX_ENABLE_CTRL_3    = 0x8304,
    PHY_88Q211X_REG_GPIO_LED_CTRL            = 0x8305,
    PHY_88Q211X_REG_LED_FUNCTION_CTRL        = 0x8310,
    PHY_88Q211X_REG_LED_POLARITY_CTRL        = 0x8311,
    PHY_88Q211X_REG_LED_TIM_INT_CTRL         = 0x8312,
    PHY_88Q211X_REG_CU_PORT_PACKET_GEN       = 0x8610,
    PHY_88Q211X_REG_CU_PORT_PACKET_SIZE      = 0x8611,
    PHY_88Q211X_REG_CHECKER_CTRL             = 0x8612,
    PHY_88Q211X_REG_PACKET_GENERATOR_CTRL    = 0x8613,
    PHY_88Q211X_REG_CU_PORT_PACKET_COUNTER   = 0x8614,
    PHY_88Q211X_REG_CU_PORT_CRC_COUNTER      = 0x8615,
    PHY_88Q211X_REG_BIST_CTRL                = 0x8617,
    PHY_88Q211X_REG_BIST_STATUS              = 0x8618,
    PHY_88Q211X_REG_BIST_COUNTERS            = 0x8619,
};

enum phy_88q211x_100base_t1_pcs_dev_e {
    PHY_88Q211X_DEV_100BASE_T1_CU_CTRL       = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_STATUS_1      = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_STATUS_2      = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_INT_EN        = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_CU_INT_STATUS = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_INT_STATUS_1  = 0x03,
    PHY_88Q211X_DEV_LINK_DROP_COUNTER        = 0x03,
    PHY_88Q211X_DEV_MAC_CTRL                 = 0x03,
    PHY_88Q211X_DEV_MAC_INT_EN               = 0x03,
    PHY_88Q211X_DEV_MAC_STATUS               = 0x03,
    PHY_88Q211X_DEV_TX_FIFO_OVERFLOW_COUNTER = 0x03,
    PHY_88Q211X_DEV_COUNTER_CTRL             = 0x03,
    PHY_88Q211X_DEV_BAD_LINK_COUNTER         = 0x03,
    PHY_88Q211X_DEV_BAD_SSD_COUNTER          = 0x03,
    PHY_88Q211X_DEV_BAD_ESD_COUNTER          = 0x03,
    PHY_88Q211X_DEV_RX_ERROR_COUNTER         = 0x03,
    PHY_88Q211X_DEV_RECEIVER_STATUS          = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_INT_EN_2      = 0x03,
    PHY_88Q211X_DEV_100BASE_T1_INT_STATUS_2  = 0x03,
    PHY_88Q211X_DEV_GPIO_TX_ENABLE_CTRL_1    = 0x03,
    PHY_88Q211X_DEV_GPIO_TX_ENABLE_CTRL_2    = 0x03,
    PHY_88Q211X_DEV_GPIO_TX_ENABLE_CTRL_3    = 0x03,
    PHY_88Q211X_DEV_GPIO_LED_CTRL            = 0x03,
    PHY_88Q211X_DEV_LED_FUNCTION_CTRL        = 0x03,
    PHY_88Q211X_DEV_LED_POLARITY_CTRL        = 0x03,
    PHY_88Q211X_DEV_LED_TIM_INT_CTRL         = 0x03,
    PHY_88Q211X_DEV_CU_PORT_PACKET_GEN       = 0x03,
    PHY_88Q211X_DEV_CU_PORT_PACKET_SIZE      = 0x03,
    PHY_88Q211X_DEV_CHECKER_CTRL             = 0x03,
    PHY_88Q211X_DEV_PACKET_GENERATOR_CTRL    = 0x03,
    PHY_88Q211X_DEV_CU_PORT_PACKET_COUNTER   = 0x03,
    PHY_88Q211X_DEV_CU_PORT_CRC_COUNTER      = 0x03,
    PHY_88Q211X_DEV_BIST_CTRL                = 0x03,
    PHY_88Q211X_DEV_BIST_STATUS              = 0x03,
    PHY_88Q211X_DEV_BIST_COUNTERS            = 0x03,
};

#define PHY_88Q211X_CU_TX_FIFO_DEPTH_SHIFT (14)
#define PHY_88Q211X_CU_TX_FIFO_DEPTH_MASK  (0x3 << PHY_88Q211X_CU_TX_FIFO_DEPTH_SHIFT)

/* ---------------------------------------------------------------------------- */
/* 3.6 1000BASE-T1 Copper Unit Advance PCS Registers */
/* ---------------------------------------------------------------------------- */

enum phy_88q211x_1000base_t1_pcs_reg_e {
    PHY_88Q211X_REG_GPIO_DATA                           = 0x8012,
    PHY_88Q211X_REG_GPIO_TRI_STATE_CTRL                 = 0x8013,
    PHY_88Q211X_REG_GPIO_INT                            = 0x8014,
    PHY_88Q211X_REG_OD_CTRL                             = 0x8015,
    PHY_88Q211X_REG_FUNCTION_CTRL                       = 0x8016,
    PHY_88Q211X_REG_LED_S_POLARITY_CTRL                 = 0x8017,
    PHY_88Q211X_REG_LED_TIMER_CTRL                      = 0x8018,
    PHY_88Q211X_REG_1000BASE_T1_PCS_CTRL                = 0xfd00,
    PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_CTRL   = 0xfd04,
    PHY_88Q211X_REG_1000BASE_T1_PACKET_GENERATOR_PARAMS = 0xfd05,
    PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_CTRL     = 0xfd07,
    PHY_88Q211X_REG_1000BASE_T1_PACKET_CHECKER_COUNT    = 0xfd08,
    PHY_88Q211X_REG_1000BASE_T1_TX_FIFO_CTRL            = 0xfd20,
};


enum phy_88q211x_1000base_t1_pcs_dev_e {
    PHY_88Q211X_DEV_GPIO_DATA                           = 0x03,
    PHY_88Q211X_DEV_GPIO_TRI_STATE_CTRL                 = 0x03,
    PHY_88Q211X_DEV_GPIO_INT                            = 0x03,
    PHY_88Q211X_DEV_OD_CTRL                             = 0x03,
    PHY_88Q211X_DEV_FUNCTION_CTRL                       = 0x03,
    PHY_88Q211X_DEV_LED_S_POLARITY_CTRL                 = 0x03,
    PHY_88Q211X_DEV_LED_TIMER_CTRL                      = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_PCS_CTRL                = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_CTRL   = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_PACKET_GENERATOR_PARAMS = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_CTRL     = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_PACKET_CHECKER_COUNT    = 0x03,
    PHY_88Q211X_DEV_1000BASE_T1_TX_FIFO_CTRL            = 0x03,

};

#define PHY_88Q211X_GMII_STEERING_MUX_A_SHIFT (7)
#define PHY_88Q211X_GMII_STEERING_MUX_A_MASK  (0x3 << PHY_88Q211X_GMII_STEERING_MUX_A_SHIFT)
#define PHY_88Q211X_GMII_STEERING_MUX_B_SHIFT (9)
#define PHY_88Q211X_GMII_STEERING_MUX_B_MASK  (0x3 << PHY_88Q211X_GMII_STEERING_MUX_B_SHIFT)
#define PHY_88Q211X_GMII_STEERING_MUX_C_SHIFT (11)
#define PHY_88Q211X_GMII_STEERING_MUX_C_MASK  (0x3 << PHY_88Q211X_GMII_STEERING_MUX_C_SHIFT)
#define PHY_88Q211X_FIBER_TX_FIFO_DEPTH_SHIFT (0)
#define PHY_88Q211X_FIBER_TX_FIFO_DEPTH_MASK  (0x3 << PHY_88Q211X_FIBER_TX_FIFO_DEPTH_SHIFT)

#define PHY_88Q211X_PACKET_GEN_EN             (1 << 3)

#define PHY_88Q211X_PACKET_CHECK_COUNTER_RST  (1 << 3)
#define PHY_88Q211X_PACKET_CHECK_EN           (1 << 2)
#define PHY_88Q211X_PACKET_CHECK_SAMPLE       (1 << 0)

#define PHY_88Q211X_PACKET_CHECK_COUNT_SHIFT  (8)
#define PHY_88Q211X_PACKET_CHECK_COUNT_MASK   (0xff << PHY_88Q211X_PACKET_CHECK_COUNT_SHIFT)
#define PHY_88Q211X_PACKET_CHECK_ERRORS_SHIFT (0)
#define PHY_88Q211X_PACKET_CHECK_ERRORS_MASK  (0xff << PHY_88Q211X_PACKET_CHECK_ERRORS_SHIFT)

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
