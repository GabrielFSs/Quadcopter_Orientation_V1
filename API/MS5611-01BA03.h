/*
 * MS5611-01BA03.h
 *
 *  Created on: Jun 10, 2023
 *  Author: Gabriel Felipe
 *  email: gabriel.felipe@ufu.br
 *
 *  -Based on MPU6050.h application by leech001 - https://github.com/leech001/MPU6050 -
 *
 *  This library is focused on defining the MS5611-01BA03 registers and
 *  reading barometer and temperature data
 *  to be used in STM32
 *
 */

#ifndef MS5611_01BA03_H_
#define MS5611_01BA03_H_

#include <stdint.h>
#include "main.h"

#define MS5611_ADDRESS		0x76	//Address 0x1110110 to CSB set on GND

//	Registers define

#define Reset_Data	 		0x1E
#define D1_OSR_256			0x40
#define D1_OSR_512			0x42
#define D1_OSR_1024			0x44
#define D1_OSR_2048			0x46
#define D1_OSR_4096			0x48
#define D2_OSR_256			0x50
#define D2_OSR_512			0x52
#define D2_OSR_1024			0x54
#define D2_OSR_2048			0x56
#define D2_OSR_4096			0x58
#define PROM_READ			0xA0

//	MS5611 structure Data Calibration

typedef struct{

		uint16_t C1;
		uint16_t C2;
		uint16_t C3;
		uint16_t C4;
		uint16_t C5;
		uint16_t C6;

}MS5611_c;

// MS5611 structure Data sensors

typedef struct {

    int32_t Pressure;
    int32_t Temp;

}MS5611_t;

// Functions Definitions for I2C Communication

extern I2C_HandleTypeDef hi2c1;

void MS5611_Reset(I2C_HandleTypeDef *hi2cx);
void MS5611_INIT(I2C_HandleTypeDef *hi2cx, MS5611_c *DataStruct);
void MS5611_READ(I2C_HandleTypeDef *hi2cx, MS5611_c *DataStruct1, MS5611_t *DataStruct2);


#endif /* MS5611_01BA03_H_ */
