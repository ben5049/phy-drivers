/*
 * phy_mdio.c
 *
 *  Created on: Dec 9, 2025
 *      Author: bens1
 *
 * Platform specific examples of MDIO access functions.
 *
 */

#include "phy_common.h"
#include "phy_mdio.h"

#ifdef STM32H573xx
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_eth.h"
#endif


#ifdef STM32H573xx

extern ETH_HandleTypeDef heth;

/* This function performs a clause 22 register read */
phy_status_t phy_read_reg_c22(uint8_t phy_addr, uint16_t reg_addr, uint16_t *data, uint32_t timeout, uint32_t clk_div) {

    uint32_t tickstart;
    uint32_t tmp_ar = 0;

    /* Check for the busy flag */
    if (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t) RESET) return PHY_BUSY;

    /* Write the MACMDIOAR register */
    tmp_ar = READ_REG(heth.Instance->MACMDIOAR);
    CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_PA, (((uint32_t) phy_addr) << ETH_MACMDIOAR_PA_Pos) & ETH_MACMDIOAR_PA_Msk);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_RDA, (((uint32_t) reg_addr) << ETH_MACMDIOAR_RDA_Pos) & ETH_MACMDIOAR_RDA_Msk);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_NTC, 1 << ETH_MACMDIOAR_NTC_Pos);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_CR, clk_div);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_RD);
    CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_C45E);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_MB);
    WRITE_REG(ETH->MACMDIOAR, tmp_ar);

    /* Wait for the busy flag */
    tickstart = HAL_GetTick();
    while (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U) {
        if (((HAL_GetTick() - tickstart) > timeout)) {
            return PHY_TIMEOUT;
        }
    }

    /* Get MACMIIDR value */
    WRITE_REG(*data, (uint16_t) heth.Instance->MACMDIODR);

    return PHY_OK;
}

/* This function performs a clause 22 register write */
phy_status_t phy_write_reg_c22(uint8_t phy_addr, uint8_t reg_addr, uint16_t data, uint32_t timeout, uint32_t clk_div) {

    uint32_t tickstart;
    uint32_t tmp_ar = 0;

    /* Check for the busy flag */
    if (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t) RESET) return PHY_BUSY;

    /* Write the register data to MACMDIODR */
    WRITE_REG(heth.Instance->MACMDIODR, data);

    /* Write the MACMDIOAR register */
    tmp_ar = READ_REG(heth.Instance->MACMDIOAR);
    CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_PA, (((uint32_t) phy_addr) << ETH_MACMDIOAR_PA_Pos) & ETH_MACMDIOAR_PA_Msk);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_RDA, (((uint32_t) reg_addr) << ETH_MACMDIOAR_RDA_Pos) & ETH_MACMDIOAR_RDA_Msk);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_NTC, 1 << ETH_MACMDIOAR_NTC_Pos);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_CR, clk_div);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_WR);
    CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_C45E);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_MB);
    WRITE_REG(ETH->MACMDIOAR, tmp_ar);

    /* Wait for the busy flag */
    tickstart = HAL_GetTick();
    while (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U) {
        if (((HAL_GetTick() - tickstart) > timeout)) {
            return PHY_TIMEOUT;
        }
    }

    return PHY_OK;
}

/* This function performs a clause 45 register read */
phy_status_t phy_read_reg_c45(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t *data, uint32_t timeout, bool preamble_supression, uint32_t clk_div) {

    uint32_t tickstart;
    uint32_t tmp_dr = 0;
    uint32_t tmp_ar = 0;

    /* Check for the busy flag */
    if (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t) RESET) return PHY_BUSY;

    /* Write the register address to the MACMDIODR register and set the data register to a known value (if 0xdead is read back later then it is likely that no data has been received) */
    tmp_dr = READ_REG(heth.Instance->MACMDIODR);
    MODIFY_REG(tmp_dr, ETH_MACMDIODR_RA, ((uint32_t) reg_addr) << ETH_MACMDIODR_RA_Pos);
    MODIFY_REG(tmp_dr, ETH_MACMDIODR_MD, ((uint32_t) 0xdead) << ETH_MACMDIODR_MD_Pos);
    WRITE_REG(heth.Instance->MACMDIODR, tmp_dr);

    /* Write the MACMDIOAR register */
    tmp_ar = READ_REG(heth.Instance->MACMDIOAR);
    if (preamble_supression) {
        SET_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    } else {
        CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    }
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_PA, (((uint32_t) phy_addr) << ETH_MACMDIOAR_PA_Pos));
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_RDA, (((uint32_t) mmd_addr) << ETH_MACMDIOAR_RDA_Pos));
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_CR, clk_div);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_RD);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_C45E);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_NTC, 1 << ETH_MACMDIOAR_NTC_Pos);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_MB);
    WRITE_REG(ETH->MACMDIOAR, tmp_ar);

    /* Wait for the busy flag */
    tickstart = HAL_GetTick();
    while (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U) {
        if (((HAL_GetTick() - tickstart) > timeout)) {
            return PHY_TIMEOUT;
        }
    }

    /* Get MACMIIDR value */
    WRITE_REG(*data, (uint16_t) heth.Instance->MACMDIODR);

    return PHY_OK;
}

/* This function performs a clause 45 register write */
phy_status_t phy_write_reg_c45(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t data, uint32_t timeout, bool preamble_supression, uint32_t clk_div) {

    uint32_t tickstart;
    uint32_t tmp_dr = 0;
    uint32_t tmp_ar = 0;

    /* Check for the busy flag */
    if (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t) RESET) return PHY_BUSY;

    /* Write the register address to MACMDIODR */
    tmp_dr = READ_REG(heth.Instance->MACMDIODR);
    MODIFY_REG(tmp_dr, ETH_MACMDIODR_RA, ((uint32_t) reg_addr) << ETH_MACMDIODR_RA_Pos);
    WRITE_REG(heth.Instance->MACMDIODR, tmp_dr);

    /* Write the data to MACMDIODR */
    MODIFY_REG(tmp_dr, ETH_MACMDIODR_MD, ((uint32_t) data) << ETH_MACMDIODR_MD_Pos);
    WRITE_REG(heth.Instance->MACMDIODR, tmp_dr);

    /* Write the MACMDIOAR register */
    tmp_ar = READ_REG(heth.Instance->MACMDIOAR);
    if (preamble_supression) {
        SET_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    } else {
        CLEAR_BIT(tmp_ar, ETH_MACMDIOAR_PSE);
    }
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_PA, (((uint32_t) phy_addr) << ETH_MACMDIOAR_PA_Pos));
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_RDA, (((uint32_t) mmd_addr) << ETH_MACMDIOAR_RDA_Pos));
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_CR, clk_div);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_WR);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_C45E);
    MODIFY_REG(tmp_ar, ETH_MACMDIOAR_NTC, 1 << ETH_MACMDIOAR_NTC_Pos);
    SET_BIT(tmp_ar, ETH_MACMDIOAR_MB);
    WRITE_REG(ETH->MACMDIOAR, tmp_ar);

    /* Wait for the busy flag */
    tickstart = HAL_GetTick();
    while (READ_BIT(heth.Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U) {
        if (((HAL_GetTick() - tickstart) > timeout)) {
            return PHY_TIMEOUT;
        }
    }

    return PHY_OK;
}

#endif
