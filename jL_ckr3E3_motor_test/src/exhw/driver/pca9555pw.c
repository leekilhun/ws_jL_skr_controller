/*
 * pca9555pw.c
 *
 *  Created on: May 29, 2023
 *      Author: gns2l
 */


#include "pca9555pw.h"
#include "cli.h"


#ifdef _USE_EXHW_PCA9555PW_EX_IO

/* Command Bytes */
#define PCA9555_CB_INPUTS_PORTS			0x00
#define PCA9555_CB_OUTPUTS_PORTS		0x02
#define PCA9555_CB_POL_INVERT_PORTS		0x04
#define PCA9555_CB_CONFIG_PORTS			0x06


#define PCA9555PW_I2C           _I2C_EXT_IO
#define PCA9555PW_CH_MAX        _EXHW_PCA9555PW_EX_IO_MAX_CH
#ifdef _USE_HW_CLI
void cliPca9555_exIo(cli_args_t *args);
#endif

static bool pca9555pw_ReadPort(uint8_t ch, uint8_t* p_ret);
static bool pca9555pw_WritePort(uint8_t ch, uint8_t port0_7,uint8_t port8_15);
static void pca9555pw_update();

pca9555_ch_t pca9555pw_exIo[PCA9555PW_CH_MAX] = {
		{ false, EX_IO_INPUT,  0x23,  0},
		{ false, EX_IO_OUTPUT, 0x27,  0},
};

static bool is_init = false;

bool pca9555pw_Init()
{
	is_init = i2cBegin(PCA9555PW_I2C, I2C_FREQ_400KHz);

	for (int i= 0; i < PCA9555PW_CH_MAX; i++ )
	{
		pca9555pw_exIo[i].is_open = true;
	}

	//gpioAttachCallbackFunc(_GPIO_I2C_INTERRUPT, pca9555pw_update);
	//

#ifdef _USE_HW_CLI
	cliAdd("pca9555_exIo", cliPca9555_exIo);
#endif

	return is_init;
}

bool pca9555pw_Recovery()
{
	return i2cRecovery(PCA9555PW_I2C);
}

bool pca9555pw_ReadPort(uint8_t ch, uint8_t* p_ret)
{
	bool ret = false;
#if 0
	if (ch < PCA9555PW_CH_MAX)
		ret=i2cReadData(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr,p_ret,2,100);
#endif

//	ret = i2cRead16Byte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, 0x00, p_ret, 100); // ng
	ret = i2cReadByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_INPUTS_PORTS, &p_ret[0], 100);
	ret = i2cReadByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_INPUTS_PORTS + 1, &p_ret[1], 100);

#if 0
			/* reading the low byte */
				ret = HAL_I2C_Mem_Read(hdev->hi2c, hdev->addr, addr, 1, &datahigh, 1, HAL_MAX_DELAY);
				if (ret != HAL_OK) return ret;

				/* reading the high byte */
				ret = HAL_I2C_Mem_Read(hdev->hi2c, hdev->addr, addr+1, 1, &datalow, 1, HAL_MAX_DELAY);
				if (ret != HAL_OK) return ret;
#endif
	return ret;
}

bool pca9555pw_WritePort(uint8_t ch,  uint8_t port0_7,uint8_t port8_15)
{
	bool i2c_ret = false;
	if (ch < PCA9555PW_CH_MAX)
	{
		if (pca9555pw_exIo[ch].dev_type !=EX_IO_OUTPUT)
			return i2c_ret;

		i2c_ret = i2cWriteByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_OUTPUTS_PORTS, port0_7, 100);

		i2c_ret = i2cWriteByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_OUTPUTS_PORTS + 1, port8_15, 100);
	}

	//i2c_ret = i2cWriteData(PCA9555PW_I2C, pca9555pw_exIo[ch].dev_addr, data, 3, 100);// ret OK

	// i2c_ret = i2cWriteBytes(PCA9555PW_I2C, pca9555pw_exIo[ch].dev_addr, 0x02, data, 2, 100); //ret ok

	/*
	i2c_ret = i2cWriteBytes(PCA9555PW_I2C, pca9555pw_exIo[ch].dev_addr, 0x02, data, 1, 100);
	i2c_ret = i2cWriteBytes(PCA9555PW_I2C, pca9555pw_exIo[ch].dev_addr, 0x02+1, &data[1], 1, 100);
	 */


#if 0
	static HAL_StatusTypeDef pca9555_writeRegister(PCA9555_ HandleTypeDef *hdev, uint8_t regAddr, uint16_t regValue)
	{
		HAL_StatusTypeDef ret;
		uint8_t lowb = lowByte(regValue);
		uint8_t highb = highByte(regValue);

		ret = HAL_I2C_Mem_Write(hdev->hi2c, hdev->addr, regAddr, 1, (uint8_t*) &lowb, 1, HAL_MAX_DELAY);
		if (ret != HAL_OK) return ret;

		ret = HAL_I2C_Mem_Write(hdev->hi2c, hdev->addr, regAddr+1, 1, (uint8_t*) &highb, 1, HAL_MAX_DELAY);
		if (ret != HAL_OK) return ret;

		return HAL_OK;
	}
#endif

	return i2c_ret;
}



bool pca9555pw_Read(uint8_t ch, uint16_t* p_ret)
{
	bool ret = false;
	if (ch < PCA9555PW_CH_MAX)
	{
		pca9555pw_update();

		*p_ret  = pca9555pw_exIo[ch].port_state;
		ret = true;
	}
	return ret;
}



bool pca9555pw_Write(uint8_t ch, uint16_t data)
{
	bool ret = false;
		if (ch < PCA9555PW_CH_MAX)
		{
			if (pca9555pw_exIo[ch].dev_type ==EX_IO_OUTPUT)
			{
				pca9555pw_exIo[ch].port_state = data;
				uint8_t port_0_7 = (uint8_t)data;
			  uint8_t port_8_15= (uint8_t)(data>>8);
				pca9555pw_WritePort(ch,port_0_7,port_8_15);
				ret = true;
			}
		}
		return ret;
}


void pca9555pw_update()
{
	uint16_t reverse_in_data= 0, out_data = 0;
	pca9555pw_ReadPort(_PCA9555PW_EX_IO_CH_IN, (uint8_t*)&reverse_in_data );
	pca9555pw_exIo[_PCA9555PW_EX_IO_CH_IN].port_state = ~(reverse_in_data);

	pca9555pw_ReadPort(_PCA9555PW_EX_IO_CH_OUT, (uint8_t*)&out_data);
	pca9555pw_exIo[_PCA9555PW_EX_IO_CH_OUT].port_state = ~(out_data);
}



bool pca9555pw_mode(uint8_t ch,  uint8_t port0_7,uint8_t port8_15)
{
	bool i2c_ret = false;
	if (ch < PCA9555PW_CH_MAX)
	{
		if (pca9555pw_exIo[ch].dev_type !=EX_IO_OUTPUT)
			return i2c_ret;

		i2c_ret = i2cWriteByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_POL_INVERT_PORTS, port0_7, 100);

		i2c_ret = i2cWriteByte(PCA9555PW_I2C,pca9555pw_exIo[ch].dev_addr, PCA9555_CB_POL_INVERT_PORTS + 1, port8_15, 100);
	}

	return i2c_ret;
}


#ifdef _USE_HW_CLI

void cliPca9555_exIo(cli_args_t *args)
{
	bool ret = true;
	bool pca_ret = false;
	uint8_t print_ch;
	uint8_t ch;

	uint8_t ret_data[2] = {0,} ;
	if (args->argc == 2)
	{
		if(args->isStr(0, "read") == true)
		{
			print_ch = (uint16_t) args->getData(1);

			print_ch = constrain(print_ch, 1, PCA9555PW_CH_MAX);
			ch       = print_ch - 1;

			pca_ret = pca9555pw_ReadPort(ch, &ret_data[0]);

			if (pca_ret == true)
			{
				cliPrintf("%d PCA9555 READ : 0x%02X, 0x%02X\n", print_ch, ret_data[0], ret_data[1]);

			}
			else
			{
				cliPrintf("%d PCA9555 READ - Fail \n", print_ch);
			}


		}
		else
		{
			ret = false;
		}
	}
	else if (args->argc == 4)
	{
		if(args->isStr(0, "write") == true)
		{
			print_ch = (uint16_t) args->getData(1);
			print_ch = constrain(print_ch, 1, PCA9555PW_CH_MAX);
			uint8_t port_0_7;
			uint8_t port_8_15;
			port_0_7 = args->getData(2);
			port_8_15   = args->getData(3);
			ch       = print_ch - 1;

			pca_ret = pca9555pw_WritePort(ch, port_0_7, port_8_15);

			if (pca_ret == true)
			{
				cliPrintf("%d PCA9555 WRITE : 0x%02X, 0x%02X\n", print_ch, port_0_7,port_8_15);

			}
			else
			{
				cliPrintf("%d PCA9555 WRITE - Fail \n", print_ch);
			}

		}

	}
	else if(args->isStr(0, "mode") == true)
	{
		print_ch = (uint16_t) args->getData(1);
		print_ch = constrain(print_ch, 1, PCA9555PW_CH_MAX);
		uint8_t port_0_7;
		uint8_t port_8_15;
		port_0_7 = args->getData(2);
		port_8_15   = args->getData(3);
		ch       = print_ch - 1;

		pca_ret = pca9555pw_mode(ch, port_0_7, port_8_15);

		if (pca_ret == true)
		{
			cliPrintf("%d PCA9555 mode OK \n", print_ch);

		}
		else
		{
			cliPrintf("%d PCA9555 READ - Fail \n", print_ch);
		}


	}
	else
	{
		ret = false;
	}

	if (ret == false)
	{
		cliPrintf( "pca9555_exIo read [ch 1~2]\n");
		cliPrintf( "pca9555_exIo mode [ch 1~2]\n");

		cliPrintf( "pca9555_exIo write [ch 1~2] [port0_7] [port8_15]\n");

	}
}

#endif

#endif
