/*
 * 88q211x.c
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/phy_utils.h"
#include "internal/88q211x/88q211x_regs.h"


/* Gets the temperature in degrees */
phy_status_t PHY_88Q211X_ReadTemperature(phy_handle_88q211x_t *dev, int16_t *temp) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the temperature register */
    PHY_READ_REG(PHY_88Q211X_DEV_TEMP_4, PHY_88Q211X_REG_TEMP_4, &reg_data);
    PHY_CHECK_END;

    /* Convert the temperature to degrees */
    *temp = ((int16_t) ((reg_data & PHY_88Q211X_TEMPERATURE_MASK) >> PHY_88Q211X_TEMPERATURE_SHIFT)) - 75;

end:

    PHY_UNLOCK;
    return status;
}
