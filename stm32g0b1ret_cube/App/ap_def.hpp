/*
 * ap_def.hpp
 *
 *  Created on: Mar 19, 2023
 *      Author: gns2l
 */

#ifndef INCULDE_AP_DEF_HPP_
#define INCULDE_AP_DEF_HPP_

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "stdlib.h"

#include "main.h"







#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#ifndef CMAX
#define CMAX(a,b) (((a) > (b)) ? (a) : (b))
#define CMIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


#ifndef CMAP
#define CMAP(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#endif /* INCULDE_AP_DEF_HPP_ */
