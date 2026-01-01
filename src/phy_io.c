/*
 * phy_io.c
 *
 *  Created on: Jan 1, 2026
 *      Author: bens1
 *
 * Functions to handle all MDIO reads and writes.
 *
 */

#include "phy_common.h"
#include "internal/phy_utils.h"
#include "internal/phy_io.h"


phy_status_t phy_read_reg(phy_handle_base_t *dev, uint8_t mmd_addr, uint16_t reg_addr, uint16_t *data) {

    phy_status_t status = PHY_OK;

    /* Invalid MMD address */
    if (mmd_addr > PHY_MMD_VENDOR_SPECIFIC) status = PHY_INVALID_MMD;
    PHY_CHECK_RET(status);

    /* No clause 45 support */
    if (!dev->config.c45_en) {

        /* Clause 22 direct access */
        if (mmd_addr == 0) {
            status = dev->callbacks->callback_read_reg_c22(dev->config.phy_addr, reg_addr, data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);
        }

        /* Clause 45 indirect access */
        else {

            uint16_t reg_data;

            /* Step 1:
             * Write the MMD Access Control register with the MMD Function (FNCTN) field set to 00b (address mode) and the
             * Device Address (DEVAD) field with the MDIO Management Device (MMD) address. */
            reg_data  = PHY_REGCR_FNCTN_ADDR << PHY_REGCR_FNCTN_SHIFT;
            reg_data |= mmd_addr << PHY_REGCR_DEVAD_SHIFT;
            status    = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_REGCR, reg_data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 2:
             * Write the address of the desired register to be read into the MMD Access Address/Data register. */
            status = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_ADDAR, reg_addr, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 3:
             * Write the MMD Access Control register with the MMD Function field set to 01b, 10b, or 11b. */
            reg_data  = PHY_REGCR_FNCTN_DATA << PHY_REGCR_FNCTN_SHIFT;
            reg_data |= mmd_addr << PHY_REGCR_DEVAD_SHIFT;
            status    = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_REGCR, reg_data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 4:
             * Read the contents of the MMD’s selected register from the MMD Access Address/Data register. */
            status = dev->callbacks->callback_read_reg_c22(dev->config.phy_addr, PHY_ADDAR, data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);
        }
    }

    /* Clause 45 access supported */
    else {
        status = dev->callbacks->callback_read_reg_c45(dev->config.phy_addr, mmd_addr, reg_addr, data, dev->config.timeout, dev->callback_context);
        PHY_CHECK_RET(status);
    }

    return status;
}


phy_status_t phy_write_reg(phy_handle_base_t *dev, uint8_t mmd_addr, uint16_t reg_addr, uint16_t data) {

    phy_status_t status = PHY_OK;

    /* Invalid MMD address */
    if (mmd_addr > PHY_MMD_VENDOR_SPECIFIC) status = PHY_INVALID_MMD;
    PHY_CHECK_RET(status);

    /* No clause 45 support */
    if (!dev->config.c45_en) {

        /* Clause 22 direct access */
        if (mmd_addr == 0) {
            status = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, reg_addr, data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);
        }

        /* Clause 45 indirect access */
        else {

            uint16_t reg_data;

            /* Step 1:
             * Write the MMD Access Control register with the MMD Function (FNCTN) field set to 00b (address mode) and the
             * Device Address (DEVAD) field with the MDIO Management Device (MMD) address. */
            reg_data  = PHY_REGCR_FNCTN_ADDR << PHY_REGCR_FNCTN_SHIFT;
            reg_data |= mmd_addr << PHY_REGCR_DEVAD_SHIFT;
            status    = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_REGCR, reg_data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 2:
             * Write the address of the desired register to be written into the MMD Access Address/Data register. */
            status = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_ADDAR, reg_addr, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 3:
             * Write the MMD Access Control register with the MMD Function field set to 01b, 10b, or 11b. */
            reg_data  = PHY_REGCR_FNCTN_DATA << PHY_REGCR_FNCTN_SHIFT;
            reg_data |= mmd_addr << PHY_REGCR_DEVAD_SHIFT;
            status    = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_REGCR, reg_data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);

            /* Step 4:
             * Write the contents of the MMD’s selected register into the MMD Access Address/Data register. */
            status = dev->callbacks->callback_write_reg_c22(dev->config.phy_addr, PHY_ADDAR, data, dev->config.timeout, dev->callback_context);
            PHY_CHECK_RET(status);
        }
    }

    /* Clause 45 access supported */
    else {
        status = dev->callbacks->callback_write_reg_c45(dev->config.phy_addr, mmd_addr, reg_addr, data, dev->config.timeout, dev->callback_context);
        PHY_CHECK_RET(status);
    }

    return status;
}
