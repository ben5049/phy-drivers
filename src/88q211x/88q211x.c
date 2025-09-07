/*
 * 88q211x.c
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/phy_utils.h"
#include "internal/88q211x/88q211x_regs.h"


phy_status_t PHY_88Q211X_GetLinkState(phy_handle_88q211x_t *dev, bool *linkup) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Powered down PHY can't have a link up */
    if (dev->state == PHY_STATE_88Q211X_POWER_DOWN) {
        *linkup = false;
        goto end;
    }

    /* Read link status register based on the link speed */
    if (dev->speed == PHY_SPEED_100M) {

        /* Read the status register */
        PHY_READ_REG(PHY_88Q211X_DEV_100BASE_T1_STATUS_2, PHY_88Q211X_REG_100BASE_T1_STATUS_2, &reg_data);
        PHY_CHECK_END;

        /* Extract the link status bit */
        *linkup = (bool) (reg_data & PHY_88Q211X_100BASE_T1_LINK_STATUS);

    } else if (dev->speed == PHY_SPEED_1G) {

        /* Read the status register */
        PHY_READ_REG(PHY_88Q211X_DEV_PCS_1000BASE_T1_STATUS_1, PHY_88Q211X_REG_PCS_1000BASE_T1_STATUS_1, &reg_data);
        PHY_CHECK_END;

        /* Extract the link status bit */
        *linkup = (bool) (reg_data & PHY_88Q211X_100BASE_T1_LINK_STATUS);

    } else {
        status = PHY_PARAMETER_ERROR;
        goto end;
    }

    /* Update the device struct */
    if (*linkup) {
        dev->state = PHY_STATE_88Q211X_LINK_UP;
    } else {
        dev->state = PHY_STATE_88Q211X_IDLE;
    }

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_EnableTransmit(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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


phy_status_t PHY_88Q211X_SetSpeed(phy_handle_88q211x_t *dev, phy_speed_t speed) {

    phy_status_t status       = PHY_OK;
    uint16_t     new_reg_data = 0;
    uint16_t     old_reg_data = 0;

    PHY_LOCK;

    /* Get current speed */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &old_reg_data);
    PHY_CHECK_END;

    /* Modify the register value */
    new_reg_data  = old_reg_data;
    new_reg_data &= ~PHY_88Q211X_SPEED_MASK;
    if (speed == PHY_SPEED_100M) {
        new_reg_data |= ((uint16_t) PHY_88Q211X_SPEED_100M << PHY_88Q211X_SPEED_SHIFT) & PHY_88Q211X_SPEED_MASK;
    } else if (speed == PHY_SPEED_1G) {
        new_reg_data |= ((uint16_t) PHY_88Q211X_SPEED_1000M << PHY_88Q211X_SPEED_SHIFT) & PHY_88Q211X_SPEED_MASK;
    } else {
        status = PHY_PARAMETER_ERROR;
        return status;
    }

    /* Send the new register value if it has changed */
    if (new_reg_data != old_reg_data) {
        PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, new_reg_data);
        PHY_CHECK_END;
    }

    /* Update the device struct */
    dev->speed = speed;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_GetSpeed(phy_handle_88q211x_t *dev, phy_speed_t *speed) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Get current speed */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &reg_data);
    PHY_CHECK_END;

    /* Set the speed */
    switch ((reg_data & PHY_88Q211X_SPEED_MASK) >> PHY_88Q211X_SPEED_SHIFT) {
        case (PHY_88Q211X_SPEED_100M):
            dev->speed = PHY_SPEED_100M;
            break;

        case (PHY_88Q211X_SPEED_1000M):
            dev->speed = PHY_SPEED_1G;
            break;

        default:
            status = PHY_INVALID_REGISTER_CONTENT_ERROR;
            break;
    }
    PHY_CHECK_END;

    /* Write the output */
    if (speed != NULL) *speed = dev->speed;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_GetDuplex(phy_handle_88q211x_t *dev, phy_duplex_t *duplex) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    PHY_LOCK;

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_SetRole(phy_handle_88q211x_t *dev, phy_role_t role) {

    phy_status_t status       = PHY_OK;
    uint16_t     new_reg_data = 0;
    uint16_t     old_reg_data = 0;

    PHY_LOCK;

    /* Get current speed */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &old_reg_data);
    PHY_CHECK_END;

    /* Modify the register value */
    new_reg_data = old_reg_data;
    if (role == PHY_ROLE_MASTER) {
        new_reg_data |= PHY_88Q211X_MASTER;
    } else if (role == PHY_ROLE_SLAVE) {
        new_reg_data &= ~PHY_88Q211X_MASTER;
    } else {
        status = PHY_PARAMETER_ERROR;
        return status;
    }

    /* Send the new register value if it has changed */
    if (new_reg_data != old_reg_data) {
        PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, new_reg_data);
        PHY_CHECK_END;
    }

    /* Update the device struct */
    dev->role = role;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_GetRole(phy_handle_88q211x_t *dev, phy_role_t *role) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Get current role */
    PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &reg_data);
    PHY_CHECK_END;

    /* Set the role */
    if (reg_data & PHY_88Q211X_MASTER) {
        dev->role = PHY_ROLE_MASTER;
    } else {
        dev->role = PHY_ROLE_SLAVE;
    }

    /* Write the output */
    if (role != NULL) *role = dev->role;

end:
    PHY_UNLOCK;
    return status;
}


/* Enables the temperature sensor with default settings (1Hz ODR) */
phy_status_t PHY_88Q211X_EnableTemperatureSensor(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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

    /* Update the device struct */
    dev->state = PHY_STATE_88Q211X_POWER_DOWN;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_DisableIEEEPowerDown(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

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

    /* Update the device struct */
    dev->state = PHY_STATE_88Q211X_IDLE;

end:
    PHY_UNLOCK;
    return status;
}