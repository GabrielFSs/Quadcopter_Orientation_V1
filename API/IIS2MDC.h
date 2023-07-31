/*
 * IIS2MDC.h
 *
 *  Created on: Jun 10, 2023
 *  Author: Gabriel Felipe
 *  email: gabriel.felipe@ufu.br
 *
 *  -Based on MPU6050.h application by leech001 - https://github.com/leech001/MPU6050 -
 *
 *  This library is focused on defining the IIS2MDC registers,
 *  reading the magnetometer data and defining the interrupt parameters
 *  to be used in STM32
 *
 */

#ifndef IIS2MDC_H_
#define IIS2MDC_H_

#include <stdint.h>
#include "main.h"

#define IIS2MDC_ADDRESS 0x1E		//Address 0011110 = 0x1E

// Register defines

#define OFFSET_X_REG_L 		0x45
#define OFFSET_X_REG_H		0x46
#define	OFFSET_Y_REG_L		0x47
#define	OFFSET_Y_REG_H		0x48
#define	OFFSET_Z_REG_L		0x49
#define	OFFSET_Z_REG_H		0x4A
#define	WHO_AM_I_M			0x4F
#define	CFG_REG_A			0x60
#define	CFG_REG_B			0x61
#define	CFG_REG_C			0x62
#define	INT_CRTL_REG		0x63
#define	INT_SOURCE_REG		0x64
#define	INT_THS_L_REG		0x65
#define	INT_THS_H_REG		0x66
#define	STATUS_REG			0x67
#define	OUTX_L_REG			0x68
#define	OUTX_H_REG			0x69
#define	OUTY_L_REG			0x6A
#define	OUTY_H_REG			0x6B
#define	OUTZ_L_REG			0x6C
#define	OUTZ_H_REG			0x6D
#define	TEMP_OUT_L_REG		0x6E
#define	TEMP_OUT_H_REG		0x6F


// IIS2MDC structure data

typedef struct {

    int16_t Mag_X;
    int16_t Mag_Y;
    int16_t Mag_Z;

} IIS2MDC_t;


// Functions Definitions for I2C Communication


extern I2C_HandleTypeDef hi2c1;

void IIS2MDC_INIT(I2C_HandleTypeDef *I2Cx);
void IIS2MDC_Read_MAG(I2C_HandleTypeDef *I2Cx, IIS2MDC_t *DataStruct);

#endif /* IIS2MDC_H_ */
