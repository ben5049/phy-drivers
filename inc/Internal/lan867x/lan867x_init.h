/*
 * lan867x_init.h
 *
 *  Created on: Dec 9, 2025
 *      Author: bens1
 */

#ifndef INC_LAN867X_INIT_H_
#define INC_LAN867X_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "lan867x.h"


#define LAN867X_CLEAR_STATE(dev)    \
    do {                            \
        dev->linkup        = false; \
        dev->write_enabled = true;  \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif /* INC_LAN867X_INIT_H_ */
