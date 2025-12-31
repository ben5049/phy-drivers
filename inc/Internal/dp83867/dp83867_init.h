/*
 * dp83867_init.h
 *
 *  Created on: Dec 31, 2025
 *      Author: bens1
 */

#ifndef INC_DP83867_INIT_H_
#define INC_DP83867_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "dp83867.h"


#define DP83867_CLEAR_STATE(dev)    \
    do {                            \
        dev->linkup        = false; \
        dev->write_enabled = true;  \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif /* INC_DP83867_INIT_H_ */
