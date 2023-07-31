/*
 * IIS2MDC.c
 *
 *  Created on: Jun 10, 2023
 *      Author: Gabriel Felipe
 */

#include <IIS2MDC.h>
#include <stdint.h>

void IIS2MDC_INIT(I2C_HandleTypeDef *hi2cx)
{
    uint8_t Data;

    // Set the parameters about the CFG_REG_A to set mode operation, enable the temperature compensation, and set the ODR to 10hz, register 0x60 = 0x83

    Data = 0x83;
    HAL_I2C_Mem_Write(hi2cx, IIS2MDC_ADDRESS, CFG_REG_A, 1, &Data, 1, 10);

    // Set the parameters about the CFG_REG_C register 0x62 to enable the Interrupt pin to data-ready and self test, register 0x62 = 0x3

    Data = 0x3;
    HAL_I2C_Mem_Write(hi2cx, IIS2MDC_ADDRESS, CFG_REG_C, 1, &Data, 1, 10);

    // Set when the the interrupt is activated, to when XYZ data ready, 0x67 = 00001000 = 0x8

    Data = 0x8;
    HAL_I2C_Mem_Write(hi2cx, IIS2MDC_ADDRESS, STATUS_REG, 1, &Data, 1, 10);
}

void IIS2MDC_Read_MAG(I2C_HandleTypeDef *hi2cx, IIS2MDC_t *DataStruct)
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from OUTX_L_REG register, 0x68 and put on Rec_Data (HSB to LSB) and put on Rec_Data

    HAL_I2C_Mem_Read(hi2cx, IIS2MDC_ADDRESS, OUTX_L_REG, 1, Rec_Data, 6, 10);

    DataStruct->Mag_X = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Mag_Y = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Mag_Z = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
}
