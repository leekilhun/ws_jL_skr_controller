/*
 * exhw.c
 *
 *  Created on: 2022. 7. 24.
 *      Author: gns2l
 */



#include "exhw.h"


bool exhwInit(void)
{
	bool ret = true;

#ifdef _USE_EXHW_AT24C64
	ret = at24c64Init();
#endif


#ifdef _USE_EXHW_KDR10K_IDSW
	ret = kdr10K_IDswInit();
#endif

#ifdef _USE_EXHW_PCA8575PW_EX_IO
	ret &= pca8575pw_Init();
#endif

#ifdef _USE_EXHW_PCA9555PW_EX_IO
	ret &= pca9555pw_Init();
#endif

#ifdef _USE_EXHW_MOTORS
  motorsInit();
#endif

  return ret;
}
