/*
 * fatfs.h
 *
 *  Created on: 2020. 12. 25.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_FATFS_H_
#define SRC_COMMON_HW_INCLUDE_FATFS_H_

#include "hw_def.h"


#ifdef _USE_HW_FATFS

#ifdef __cplusplus
extern "C" {
#endif

bool fatfsInit(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* SRC_COMMON_HW_INCLUDE_FATFS_H_ */
