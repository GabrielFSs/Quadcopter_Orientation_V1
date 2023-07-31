/*
 * MS5611-01BA03.c
 *
 *  Created on: Jun 10, 2023
 *      Author: Gabriel Felipe
 */

#include <stdint.h>
#include <math.h>
#include <MS5611-01BA03.h>

// Function to initiate the sensor

void MS5611_INIT(I2C_HandleTypeDef *hi2cx, MS5611_c *DataStruct)
{
	uint8_t Rec_Data[12];

    // Read 12 BYTES of data starting from PROM register 0xA0 and put on Rec_Data

    HAL_I2C_Mem_Read(hi2cx, MS5611_ADDRESS, PROM_READ, 1, Rec_Data, 12, 10);

    DataStruct->C1 = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->C2 = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->C3 = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
    DataStruct->C4 = (int16_t) (Rec_Data[6] << 8 | Rec_Data[7]);
    DataStruct->C5 = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
    DataStruct->C6 = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);

}

// Function to reset the sensor

void MS5611_Reset(I2C_HandleTypeDef *hi2cx)
{

	// Send the reset command to MS5611

	uint8_t data = Reset_Data;
	HAL_I2C_Master_Transmit(hi2cx, MS5611_ADDRESS, &data, 1, 10);
}

// Function for read and convert the data using the math forms in the DataSheet

void MS5611_READ(I2C_HandleTypeDef *hi2cx,  MS5611_c *DataStruct1, MS5611_t *DataStruct2)
{
	uint8_t Rec_data[3];
	uint32_t dados_p;
	uint32_t dados_t;
	int32_t dT;
	int64_t OFF;
	int64_t SENS;
	uint8_t D1_OSR = D1_OSR_4096;
	uint8_t D2_OSR = D2_OSR_4096;

	// Send directly to MS5611 the command to start the pressure conversion OSR=4096

	HAL_I2C_Master_Transmit(hi2cx, MS5611_ADDRESS, &D1_OSR, 1, 10);
	int32_t tempo = HAL_GetTick();

	if(HAL_GetTick() >= tempo + 10)
	{
		// Read the pressure data

		HAL_I2C_Mem_Read(hi2cx, MS5611_ADDRESS, 0x00, 1, Rec_data, 3, 10);
	}

	dados_p = (uint32_t) (Rec_data[0] << 16 | Rec_data[1] << 8 | Rec_data[0]);

	// Send directly to MS5611 the command to start the pressure conversion OSR=4096

	HAL_I2C_Master_Transmit(hi2cx, MS5611_ADDRESS, &D2_OSR, 1, 10);
	tempo = HAL_GetTick();

	if(HAL_GetTick() >= tempo + 10)
	{
		// Read the temperature data

		HAL_I2C_Mem_Read(hi2cx, MS5611_ADDRESS, 0x00, 1, Rec_data, 3, 10);
	}

	// Do the math calculations

	dados_t = (uint32_t) (Rec_data[0] << 16 | Rec_data[1] << 8 | Rec_data[0]);

	dT = (dados_t - DataStruct1->C5 * 256);
	DataStruct2->Temp = (2000 + dT * DataStruct1->C6 / 8388608);
	OFF = (DataStruct1->C2 * 65536 + (DataStruct1->C4 * dT) / 128);
	SENS = (DataStruct1->C1 * 32768 + (DataStruct1->C3 * dT) / 256);
	DataStruct2->Pressure = ((dados_p * SENS / 2097152 - OFF) /32768);
}
