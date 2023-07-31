/*
 * app.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Gabriel Felipe
 */

#include "app.h"
#include "main.h"
#include <ICM-42688-P.h>
#include <IIS2MDC.h>
#include <MS5611-01BA03.h>
#include <NEO-M9N.h>
#include <NMEA.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "usb_device.h"
#include "usbd_cdc_if.h"

extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim1;

// Data Structures and Buffers

static MS5611_t MS5611_Data;
static MS5611_c Calibration_Data;
static ICM42688_t ICM42688_Data;
static IIS2MDC_t IIS2MDS_Data;
static GPSSTRUCT gps_Pre_Data;
static GPSSTRUCT gps_Valid_Data;

char GGA[100];
char RMC[100];

void setup(void)
{
	// Initialize the Timers

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim10);

	// Initializes the communication of the sensors, and their initial settings

	ICM42688_INIT(&hi2c1);
	IIS2MDC_INIT(&hi2c1);
	MS5611_INIT(&hi2c1, &Calibration_Data);
	Ringbuf_init();
}

void loop(void)
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim == &htim10)
   {
	   //Read the MS5611 data ever the Timer 10 make an interrupt every 200ms

	   MS5611_READ(&hi2c1, &Calibration_Data, &MS5611_Data);

	   // Checks for GGA or RMC data via UART
	   //M9N_Read();
   }

   if(htim == &htim1)
   {
	   //Execute the data print function every 1000ms

	   Data_SerialPrint();
   }

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ICM_INT_Pin)
	{
		// Read the ICM42611 data when the sensor generates an interrupt when they have a data ready

		ICM42688_Read_ACCEL(&hi2c1, &ICM42688_Data);
		ICM42688_Read_GYRO(&hi2c1, &ICM42688_Data);
	}

	//if(GPIO_Pin == MAG_INT_Pin)
	{
		// Read the IIS2MDC data when the sensor generates an interrupt when they have a data ready

		IIS2MDC_Read_MAG(&hi2c1, &IIS2MDS_Data);
	}
}

void M9N_Read(void)
{
	// checks when the GPS sensor send the GGA or RMC data

	if (Wait_for("GGA") == 1)
	  {
		// When is received the GGA data, its copy the data to GGA buffer

		Copy_upto("*", GGA);

		// Check if the data is valid, and save in the gps_Valid_Data structure the data read

		  if (decodeGGA(GGA, &gps_Pre_Data.ggastruct) == 0)
		  {
			  gps_Valid_Data = gps_Pre_Data;
		  }
	  }

	  if (Wait_for("RMC") == 1)
	  {
		// When is received the RMC data, its copy the data to RMC buffer

		  Copy_upto("*", RMC);

		 // Check if the data is valid, and save in the gps_Valid_Data structure the data read

		  if (decodeRMC(RMC, &gps_Pre_Data.rmcstruct) == 0)
		  {
			  gps_Valid_Data = gps_Pre_Data;
		  }
	  }
}

void Data_SerialPrint(void)
{
    //Use the usbd_cdc library to print the data via USB CDC in the serial port

	char buffer[256];
    sprintf(buffer, "Temperatura: %lu "
    				"\nPressao: %lu "
    				"\nGyroscope: X = %.2f, Y = %.2f, Z = %.2f "
    				"\nAccelerometer: X = %.2f, Y = %.2f, Z = %.2f "
    				"\nMagnetometer: X = %d, Y = %d, Z = %d "
    				"\n//////////////////////////////////\n",
					MS5611_Data.Temp,
					MS5611_Data.Pressure,
					ICM42688_Data.Gx, ICM42688_Data.Gy, ICM42688_Data.Gz,
					ICM42688_Data.Ax, ICM42688_Data.Ay, ICM42688_Data.Az,
					IIS2MDS_Data.Mag_X, IIS2MDS_Data.Mag_Y, IIS2MDS_Data.Mag_Z);


    CDC_Transmit_FS((uint8_t*)buffer, strlen(buffer));

}
