/*
 * pca9555pw.h
 *
 *  Created on: May 29, 2023
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_EXHW_PCA9555PW_H_
#define SRC_COMMON_INC_EXHW_PCA9555PW_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_PCA9555PW_EX_IO

	typedef enum
	{
	  EX_IO_INPUT,
		EX_IO_OUTPUT,
	} ex_io_t;

  typedef struct
  {
    bool      is_open;
    ex_io_t   dev_type;
    uint8_t   dev_addr;
    uint16_t  port_state;
  }pca9555_ch_t;

  extern pca9555_ch_t data[_EXHW_PCA9555PW_EX_IO_MAX_CH];

bool pca9555pw_Init();
bool pca9555pw_Recovery();
bool pca9555pw_Read(uint8_t ch, uint16_t* p_ret);
bool pca9555pw_Write(uint8_t ch, uint16_t data);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_INC_EXHW_PCA9555PW_H_ */
