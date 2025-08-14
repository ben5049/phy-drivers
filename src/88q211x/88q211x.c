/*
 * 88q211x.c
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/phy_utils.h"
#include "internal/88q211x/88q211x_regs.h"


/* Enables the temperature sensor with default settings (1Hz ODR) */
phy_status_t PHY_88Q211X_EnableTemperatureSensor(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the temperature sensor control register */
    PHY_READ_REG(PHY_88Q211X_DEV_TEMP_3, PHY_88Q211X_REG_TEMP_3, &reg_data);
    PHY_CHECK_END;

    /* If the sensor isn't configured in 1Hz mode then configure it */
    if (((reg_data & PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK) >> PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT) != PHY_88Q211X_TEMPERATURE_SENSOR_1HZ) {
        reg_data &= ~PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK;
        reg_data |= ((uint16_t) PHY_88Q211X_TEMPERATURE_SENSOR_1HZ << PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT) & PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK;
        PHY_WRITE_REG(PHY_88Q211X_DEV_TEMP_3, PHY_88Q211X_REG_TEMP_3, reg_data);
        PHY_CHECK_END;
    }

end:

    PHY_UNLOCK;
    return status;
}


/* Gets the instantaneous temperature in degrees. Note the temperature sensor must be enabled first */
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
