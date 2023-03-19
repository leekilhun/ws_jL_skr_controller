/*
 * ap.hpp
 *
 *  Created on: Mar 13, 2023
 *      Author: gns2l
 */

#ifndef AP_HPP_
#define AP_HPP_

#include "ap_def.hpp"

#include <array>
#include <cstdint>
#include <cstring>


#include "ap_util.hpp"
#include "tmc2209.hpp"
#include "StepMotor.hpp"


void apInit(void);
void apMain(void);



#endif /* AP_HPP_ */
