/*
 * 88q211x.c
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/phy_utils.h"
#include "internal/88q211x/88q211x_regs.h"


phy_status_t PHY_88Q211X_EnableTransmit(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END;

    /* If transmit is already enabled then return */
    if (!(reg_data & PHY_88Q211X_PMA_TRANSMIT_DISABLE)) goto end;

    /* Otherwise set the transmit disable bit to 0 */
    reg_data &= ~PHY_88Q211X_PMA_TRANSMIT_DISABLE;
    PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
    PHY_CHECK_END;

end:
    PHY_UNLOCK;
    return status;
}
phy_status_t PHY_88Q211X_DisableTransmit(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END;

    /* If transmit is already disabled then return */
    if (reg_data & PHY_88Q211X_PMA_TRANSMIT_DISABLE) goto end;

    /* Otherwise set the transmit disable bit to 1 */
    reg_data |= PHY_88Q211X_PMA_TRANSMIT_DISABLE;
    PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
    PHY_CHECK_END;

end:
    PHY_UNLOCK;
    return status;
}

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


phy_status_t PHY_88Q211X_EnableIEEEPowerDown(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END;

    /* If not powered down the set the low power bit */
    if (!(reg_data & PHY_88Q211X_IEEE_POWER_DOWN)) {
        reg_data |= PHY_88Q211X_IEEE_POWER_DOWN;
        PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
        PHY_CHECK_END;
    }

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_DisableIEEEPowerDown(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_OK;
    uint16_t     reg_data;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END;

    /* If powered down the clear the low power bit */
    if (reg_data & PHY_88Q211X_IEEE_POWER_DOWN) {
        reg_data &= ~PHY_88Q211X_IEEE_POWER_DOWN;
        PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
        PHY_CHECK_END;
    }

    /* Read the PCS control register 1 */
    PHY_READ_REG(PHY_88Q211X_DEV_PCS_CTRL_1, PHY_88Q211X_REG_PCS_CTRL_1, &reg_data);
    PHY_CHECK_END;

    /* If powered down the clear the low power bit */
    if (reg_data & PHY_88Q211X_IEEE_POWER_DOWN) {
        reg_data &= ~PHY_88Q211X_IEEE_POWER_DOWN;
        PHY_WRITE_REG(PHY_88Q211X_DEV_PCS_CTRL_1, PHY_88Q211X_REG_PCS_CTRL_1, reg_data);
        PHY_CHECK_END;
    }

end:
    PHY_UNLOCK;
    return status;
}