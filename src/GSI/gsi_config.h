/*
 * gsi_config.h
 *
 *  Created on: 04/lug/2015
 *      Author: ntonjeta
 */

#ifndef GSI_GSI_CONFIG_H_
#define GSI_GSI_CONFIG_H_


#include <stdio.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"

//ABILITAZIONE DELLE INTERFACCE
#define I2C_SENSOR_ENABLED
#define SPI_SENSOR_ENABLED
#define ADC_SENSOR_ENABLED

//DEFINISCI IL NUMERO DI SENSORI
#define SENSOR_NUMBER 3


//Configura primo sensore
#define  ID_1 		1
#define  CLASS_1	6
#define  LINK_1		1
#define  ADDRESS_1	10
#define  PERIOD_1	1000

//Configura sencondo sensore
#define  ID_2 		2
#define  CLASS_2	6
#define  LINK_2		2
#define  ADDRESS_2	0
#define  PERIOD_2	1000

//Configura terzo sensore
#define  ID_3 		3
#define  CLASS_3	1
#define  LINK_3		4
#define  ADDRESS_3	0
#define  PERIOD_3	0






#endif /* GSI_GSI_CONFIG_H_ */
