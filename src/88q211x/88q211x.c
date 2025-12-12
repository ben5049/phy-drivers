/*
 * 88q211x.c
 *
 *  Created on: Aug 3, 2025
 *      Author: bens1
 */

#include "88q211x.h"
#include "internal/phy_utils.h"
#include "internal/88q211x/88q211x_regs.h"
#include "internal/88q211x/88q211x_bist.h"


phy_status_t PHY_88Q211X_EnableInterrupts(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Read the interrupt pin config register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_LED_TIMER_CTRL, PHY_88Q211X_REG_LED_TIMER_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* Make sure the interrupt polarity is low and not forced on */
    if (!(reg_data & PHY_88Q211X_INT_POLARITY_LOW) || (reg_data & PHY_88Q211X_FORCE_INT)) {
        reg_data |= PHY_88Q211X_INT_POLARITY_LOW;
        reg_data &= ~PHY_88Q211X_FORCE_INT;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_LED_TIMER_CTRL, PHY_88Q211X_REG_LED_TIMER_CTRL, reg_data);
        PHY_CHECK_END(status);
    }

    /* Read the GPIO tri-state register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_GPIO_TRI_STATE_CTRL, PHY_88Q211X_REG_GPIO_TRI_STATE_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* Disable tri-state mode on the interrupt pin and configure as open drain */
    if (!(reg_data & PHY_88Q211X_INT_DISABLE_TRI_STATE) ||
        (reg_data & PHY_88Q211X_INT_OPEN_SOURCE) ||
        !(reg_data & PHY_88Q211X_INT_OPEN_DRAIN)) {

        reg_data |= PHY_88Q211X_INT_DISABLE_TRI_STATE;
        reg_data &= ~PHY_88Q211X_INT_OPEN_SOURCE;
        reg_data |= PHY_88Q211X_INT_OPEN_DRAIN;

        status = PHY_WRITE_REG(PHY_88Q211X_DEV_GPIO_TRI_STATE_CTRL, PHY_88Q211X_REG_GPIO_TRI_STATE_CTRL, reg_data);
        PHY_CHECK_END(status);
    }

    /* Configure which general interrupts are enabled */
    reg_data  = 0;
    reg_data |= PHY_88Q211X_INT_PMT_LINK_UP;
    reg_data |= PHY_88Q211X_INT_PMT_LINK_DOWN;
    reg_data |= PHY_88Q211X_INT_100BASE_T1;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_INT_EN_1, PHY_88Q211X_REG_INT_EN_1, reg_data);
    PHY_CHECK_END(status);

    /* Configure 100BASE-T1 specific interrupts. TODO: Enable error interrupts */
    // reg_data  = 0;
    // reg_data |= PHY_88Q211X_100BASE_T1_LINK_STATUS_CHANGE;
    // status = PHY_WRITE_REG(PHY_88Q211X_DEV_100BASE_T1_INT_EN_1, PHY_88Q211X_REG_100BASE_T1_INT_EN_1, reg_data);
    // PHY_CHECK_END(status);

    /* Configure 100BASE-T1 MAC specific interrupts. TODO: Enable error interrupts */
    // reg_data = 0;
    // status = PHY_WRITE_REG(PHY_88Q211X_DEV_MAC_INT_EN, PHY_88Q211X_REG_MAC_INT_EN, reg_data);
    // PHY_CHECK_END(status);

    /* TODO: Configure SGMII specific interrupts. TODO: Enable error interrupts */

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_DisableInterrupts(phy_handle_88q211x_t *dev) {

    phy_status_t status = PHY_NOT_IMPLEMENTED_ERROR;

    PHY_LOCK;

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_ProcessInterrupt(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Get the interrupt status bits */
    status = PHY_READ_REG(PHY_88Q211X_DEV_GPIO_INT_STATUS, PHY_88Q211X_REG_GPIO_INT_STATUS, &reg_data);
    PHY_CHECK_END(status);

    /* Get the link up or down status */
    if ((reg_data & PHY_88Q211X_INT_PMT_LINK_UP) && !(reg_data & PHY_88Q211X_INT_PMT_LINK_DOWN)) {
        dev->linkup = true;
        status      = dev->callbacks->callback_event(PHY_EVENT_LINK_UP, dev->callback_context);
        PHY_CHECK_END(status);
    } else if (!(reg_data & PHY_88Q211X_INT_PMT_LINK_UP) && (reg_data & PHY_88Q211X_INT_PMT_LINK_DOWN)) {
        dev->linkup = false;
        status      = dev->callbacks->callback_event(PHY_EVENT_LINK_DOWN, dev->callback_context);
        PHY_CHECK_END(status);
    }

    /* If the link is both up and down then read the status registers to check which is true */
    else if ((reg_data & PHY_88Q211X_INT_PMT_LINK_UP) && (reg_data & PHY_88Q211X_INT_PMT_LINK_DOWN)) {
        status = PHY_88Q211X_GetLinkState(dev, NULL); /* This calls dev->callback_event(PHY_EVENT_LINK_xxxx, ...) */
        PHY_CHECK_END(status);
    }

    /* Unknown interrupt */
    else {
        status = PHY_UNKNOWN_INTERRUPT_ERROR;
        PHY_CHECK_END(status);
    }

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_GetLinkState(phy_handle_88q211x_t *dev, bool *linkup) {

    phy_status_t status          = PHY_OK;
    uint16_t     reg_data        = 0;
    bool         linkup_internal = false;

    PHY_LOCK;

    /* Powered down PHY can't have a link up */
    if (dev->state == PHY_STATE_88Q211X_POWER_DOWN) {
        if (linkup != NULL) *linkup = false;
        dev->linkup = false;
        goto end;
    }

    /* Read link status register based on the link speed */
    if (dev->speed == PHY_SPEED_100M) {

        /* Read the status register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_100BASE_T1_STATUS_2, PHY_88Q211X_REG_100BASE_T1_STATUS_2, &reg_data);
        PHY_CHECK_END(status);

        /* Extract the link status bit */
        linkup_internal = (bool) (reg_data & PHY_88Q211X_100BASE_T1_LINK_STATUS);

    } else if (dev->speed == PHY_SPEED_1G) {

        /* Read the status register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_PCS_1000BASE_T1_STATUS_1, PHY_88Q211X_REG_PCS_1000BASE_T1_STATUS_1, &reg_data);
        PHY_CHECK_END(status);

        /* Extract the link status bit */
        linkup_internal = (bool) (reg_data & PHY_88Q211X_100BASE_T1_LINK_STATUS);

    } else {
        status = PHY_PARAMETER_ERROR;
        goto end;
    }

    /* If there is a change then call the corresponding callback */
    if (dev->linkup != linkup_internal) {
        status = dev->callbacks->callback_event(linkup_internal ? PHY_EVENT_LINK_UP : PHY_EVENT_LINK_DOWN, dev->callback_context);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    if (linkup_internal) {
        dev->state  = PHY_STATE_88Q211X_LINK_UP;
        dev->linkup = true;
    } else {
        dev->state  = PHY_STATE_88Q211X_IDLE;
        dev->linkup = false;
    }

    /* Update the output */
    if (linkup != NULL) *linkup = dev->linkup;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_EnableTransmit(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* If transmit is already enabled then return */
    if (!(reg_data & PHY_88Q211X_PMA_TRANSMIT_DISABLE)) goto end;

    /* Otherwise set the transmit disable bit to 0 */
    reg_data &= ~PHY_88Q211X_PMA_TRANSMIT_DISABLE;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
    PHY_CHECK_END(status);

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_DisableTransmit(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* If transmit is already disabled then return */
    if (reg_data & PHY_88Q211X_PMA_TRANSMIT_DISABLE) goto end;

    /* Otherwise set the transmit disable bit to 1 */
    reg_data |= PHY_88Q211X_PMA_TRANSMIT_DISABLE;
    status    = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
    PHY_CHECK_END(status);

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
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &old_reg_data);
    PHY_CHECK_END(status);

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
        status = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, new_reg_data);
        PHY_CHECK_END(status);
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
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &reg_data);
    PHY_CHECK_END(status);

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
    PHY_CHECK_END(status);

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
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &old_reg_data);
    PHY_CHECK_END(status);

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
        status = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, new_reg_data);
        PHY_CHECK_END(status);
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
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_PMA_PMD_CTRL, PHY_88Q211X_REG_BASE_T1_PMA_PMD_CTRL, &reg_data);
    PHY_CHECK_END(status);

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

    if (!dev->temp_sensor_enabled) {

        /* Read the temperature sensor control register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_TEMP_3, PHY_88Q211X_REG_TEMP_3, &reg_data);
        PHY_CHECK_END(status);

        /* If the sensor isn't configured in 1Hz mode then configure it */
        if (((reg_data & PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK) >> PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT) != PHY_88Q211X_TEMPERATURE_SENSOR_1HZ) {
            reg_data &= ~PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK;
            reg_data |= ((uint16_t) PHY_88Q211X_TEMPERATURE_SENSOR_1HZ << PHY_88Q211X_TEMPERATURE_SENSOR_EN_SHIFT) & PHY_88Q211X_TEMPERATURE_SENSOR_EN_MASK;
            status    = PHY_WRITE_REG(PHY_88Q211X_DEV_TEMP_3, PHY_88Q211X_REG_TEMP_3, reg_data);
            PHY_CHECK_END(status);
        }

        dev->temp_sensor_enabled = true;
    }

end:

    PHY_UNLOCK;
    return status;
}


/* Gets the instantaneous temperature in degrees. Note the temperature sensor must be enabled first */
phy_status_t PHY_88Q211X_ReadTemperature(phy_handle_88q211x_t *dev, float *temp, bool *valid) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Filter out all the conditions that would invalidate the temperature reading. TODO: Check if reading the temperature in power down is possible */
    if (dev->temp_sensor_enabled && (dev->state != PHY_STATE_88Q211X_UNCONFIGURED) && (dev->state != PHY_STATE_88Q211X_POWER_DOWN)) {

        /* Read the temperature register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_TEMP_4, PHY_88Q211X_REG_TEMP_4, &reg_data);
        PHY_CHECK_END(status);

        /* Convert the temperature to degrees */
        *temp  = (float) (((int16_t) ((reg_data & PHY_88Q211X_TEMPERATURE_MASK) >> PHY_88Q211X_TEMPERATURE_SHIFT)) - 75);
        *valid = true;
    }

    /* The reading is invalid */
    else {
        *valid = false;
    }

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_CheckFaults(phy_handle_88q211x_t *dev, phy_fault_t *fault) {

    phy_status_t status         = PHY_OK;
    phy_fault_t  fault_internal = PHY_FAULT_NONE;
    uint16_t     reg_data       = 0;
    uint8_t      errors;

    PHY_LOCK;

    /* PCS Fault check */
    {

        /* Read the PCS status register */
        status = PHY_READ_REG(PHY_88Q211X_DEV_PCS_STATUS_2, PHY_88Q211X_REG_PCS_STATUS_2, &reg_data);
        PHY_CHECK_END(status);

        /* Check for PCS RX faults */
        if (reg_data & PHY_88Q211X_PCS_RX_FAULT) {
            fault_internal = PHY_FAULT_PCS_RX;
            dev->events.rx_faults++;
        }

        /* Check for PCS TX faults */
        if (reg_data & PHY_88Q211X_PCS_TX_FAULT) {
            fault_internal = PHY_FAULT_PCS_TX;
            dev->events.tx_faults++;
        }

        /* Diagnose 1000BASE-T1 fault */
        if ((fault_internal != PHY_FAULT_NONE) && (dev->speed == PHY_SPEED_100M)) {
            // TODO: Read 100BASE-T1 Status Register (0x8108)
            // TODO: Read dev 3, reg 0x8230
        }

        /* Diagnose 1000BASE-T1 fault */
        if ((fault_internal != PHY_FAULT_NONE) && (dev->speed == PHY_SPEED_1G)) {

            /* Read the 1000BASE-T1 PCS Status register */
            status = PHY_READ_REG(PHY_88Q211X_DEV_PCS_1000BASE_T1_STATUS_1, PHY_88Q211X_REG_PCS_1000BASE_T1_STATUS_1, &reg_data);
            PHY_CHECK_END(status);
            // if (!(reg_data & PHY_88Q211X_1000BASE_T1_FAULT)) {
            // }

            /* Check if the fault is due to a high bit error rate (BER) */
            if (fault_internal == PHY_FAULT_PCS_RX) {

                /* Check the FEC status register */
                status = PHY_READ_REG(PHY_88Q211X_DEV_1000BASE_T_PCS_STATUS_2, PHY_88Q211X_REG_1000BASE_T_PCS_STATUS_2, &reg_data);
                PHY_CHECK_END(status);

                /* If a high BER has been detected then report it */
                if (reg_data & PHY_88Q211X_HIGH_BER) {
                    fault_internal = PHY_FAULT_HIGH_BER;
                    // TODO: printf("High BER detected. Number of block errors (max 255) = %u\n", reg_data & PHY_88Q211X_ERRORED_BLOCKS_COUNTER_MASK);
                }
            }
        }
    }

    /* PMA Fault check: unsupported */

    /* Auto-negotiation fault check: TODO */

    /* SGMII fault check: TODO */

    /* Get the packet CRC error count and clear the packet checker counter */
    status = PHY_88Q211X_ReadPacketCheckerCounters(dev, NULL, &errors, true);
    PHY_CHECK_END(status);
    dev->events.crc_errors += errors;

    /* If an error has occured change the status to reflect it */
    if (fault_internal != PHY_FAULT_NONE) status = PHY_FAULT_DETECTED;
    *fault = fault_internal;

end:

    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_EnableIEEEPowerDown(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* If not powered down the set the low power bit */
    if (!(reg_data & PHY_88Q211X_IEEE_POWER_DOWN)) {
        reg_data |= PHY_88Q211X_IEEE_POWER_DOWN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    dev->state  = PHY_STATE_88Q211X_POWER_DOWN;
    dev->linkup = false;

end:
    PHY_UNLOCK;
    return status;
}


phy_status_t PHY_88Q211X_DisableIEEEPowerDown(phy_handle_88q211x_t *dev) {

    phy_status_t status   = PHY_OK;
    uint16_t     reg_data = 0;

    PHY_LOCK;

    /* Read the BASE-T1 control register */
    status = PHY_READ_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, &reg_data);
    PHY_CHECK_END(status);

    /* If powered down the clear the low power bit */
    if (reg_data & PHY_88Q211X_IEEE_POWER_DOWN) {
        reg_data &= ~PHY_88Q211X_IEEE_POWER_DOWN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_BASE_T1_CTRL, PHY_88Q211X_REG_BASE_T1_CTRL, reg_data);
        PHY_CHECK_END(status);
    }

    /* Read the PCS control register 1 */
    status = PHY_READ_REG(PHY_88Q211X_DEV_PCS_CTRL_1, PHY_88Q211X_REG_PCS_CTRL_1, &reg_data);
    PHY_CHECK_END(status);

    /* If powered down the clear the low power bit */
    if (reg_data & PHY_88Q211X_IEEE_POWER_DOWN) {
        reg_data &= ~PHY_88Q211X_IEEE_POWER_DOWN;
        status    = PHY_WRITE_REG(PHY_88Q211X_DEV_PCS_CTRL_1, PHY_88Q211X_REG_PCS_CTRL_1, reg_data);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    dev->state = PHY_STATE_88Q211X_IDLE;

end:
    PHY_UNLOCK;
    return status;
}
