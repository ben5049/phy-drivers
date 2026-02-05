/*
 * dp83867.c
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#include "internal/phy_utils.h"
#include "internal/phy_io.h"

#include "dp83867.h"
#include "internal/dp83867/dp83867_init.h"
#include "internal/dp83867/dp83867_regs.h"


phy_status_t PHY_DP83867_GetLinkState(phy_handle_dp83867_t *dev, bool *linkup) {


    phy_status_t status          = PHY_OK;
    uint16_t     reg_data        = 0;
    bool         linkup_internal = false;

    PHY_LOCK;

    /* Read the basic mode status register */
    status = PHY_READ_REG(dev, PHY_DP83867_MMD_BASIC, PHY_DP83867_REG_BASIC_BMSR, &reg_data);
    PHY_CHECK_END(status);

    /* Extract the link status bit */
    linkup_internal = (bool) (reg_data & PHY_DP83867_LINK_STATUS);

    /* If there is a change then call the corresponding callback */
    if (dev->linkup != linkup_internal) {
        status = dev->callbacks->callback_event(linkup_internal ? PHY_EVENT_LINK_UP : PHY_EVENT_LINK_DOWN, dev->callback_context);
        PHY_CHECK_END(status);
    }

    /* Update the device struct */
    dev->linkup = linkup_internal;

    /* Update the output */
    if (linkup != NULL) *linkup = dev->linkup;

end:

    PHY_UNLOCK;
    return status;
}