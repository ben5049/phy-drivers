/*
 * phy_mdio.h
 *
 *  Created on: Dec 9, 2025
 *      Author: bens1
 *
 * Platform specific examples of MDIO access functions.
 *
 */

#ifndef INC_PHY_MDIO_H_
#define INC_PHY_MDIO_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "phy_common.h"


#ifdef STM32H573xx
phy_status_t phy_read_reg_c22(uint8_t phy_addr, uint16_t reg_addr, uint16_t *data, uint32_t timeout, uint32_t clk_div);
phy_status_t phy_write_reg_c22(uint8_t phy_addr, uint8_t reg_addr, uint16_t data, uint32_t timeout, uint32_t clk_div);
phy_status_t phy_read_reg_c45(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t *data, uint32_t timeout, bool preamble_supression, uint32_t clk_div);
phy_status_t phy_write_reg_c45(uint8_t phy_addr, uint8_t mmd_addr, uint16_t reg_addr, uint16_t data, uint32_t timeout, bool preamble_supression, uint32_t clk_div);
#endif


#ifdef __cplusplus
}
#endif

#endif /* INC_PHY_MDIO_H_ */
