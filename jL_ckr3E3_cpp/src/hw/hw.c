/*
 * hw.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "hw.h"


bool hwInit(void)
{
  bool ret = true;


  ret &= bspInit();

  ret &= ledInit();



  return ret;
}



