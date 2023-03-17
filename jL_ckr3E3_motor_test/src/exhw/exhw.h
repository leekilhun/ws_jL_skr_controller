/*
 * exhw.h
 *
 *  Created on: 2022. 7. 24.
 *      Author: gns2l
 */

#ifndef SRC_EXHW_EXHW_H_
#define SRC_EXHW_EXHW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"

#include "at24c64.h"
#include "kdr10K_IDsw.h"
#include "pca8575pw.h"
#include "motors.h"


bool exhwInit(void);


#ifdef __cplusplus
}
#endif

#endif /* SRC_EXHW_EXHW_H_ */
