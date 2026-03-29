/*
 * phy_utils.c
 *
 *  Created on: Mar 29, 2026
 *      Author: bens1
 */

#include "stdint.h"

#include "phy_utils.h"


uint8_t reverse_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4; /* Swap nibbles */
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2; /* Swap pairs */
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1; /* Swap adjacent bits */
    return b;
}


uint32_t pack_oui(uint16_t oui_2_17, uint8_t oui_18_23) {
    uint32_t oui  = 0;
    oui          |= (uint32_t) reverse_byte((oui_2_17 & 0x00ff) >> 0) << 10;
    oui          |= (uint32_t) reverse_byte((oui_2_17 & 0xff00) >> 8) << 2;
    oui          |= (uint32_t) reverse_byte(oui_18_23) << 16;
    return oui;
}
