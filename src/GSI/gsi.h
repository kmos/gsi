/*
 * gsi.h
 *
 *  Created on: 04/lug/2015
 *      Author: ntonjeta
 */

#ifndef GSI_GSI_H_
#define GSI_GSI_H_

/**
 *  GSI.h
 *   Version: 0.1
 *
 *   Changelog:
*/


#include "gsi_config.h"


#define Calibration_Value 	11.0		// spiazzamento sul valore di temperatura

/* Exported types ------------------------------------------------------------*/

/*
 * Possibili collegamenti
 */
typedef enum {
	I2C			= 1,
	SPI			= 2,
	CATE		= 3,
	Analog		= 4
}Sensor_Link;


typedef enum{
	OP_OK	  		= 0,
	OP_ERR_ID 		= 1,
	OP_ERR_OVERUN	= 2
} OP_STATE;

/*
 * Classificazione dei sensori
 */
typedef enum {
    Temperature 	= 1,
    Pressure		= 2,
	Optical			= 3,
	Acoustic		= 4,
	Mechanical		= 5,
	Motion			= 6,
	Flow			= 7,
	Position		= 8,
	Electromagnetic = 9,
	Chemical		= 10,
	Humidity		= 11,
	Radiation		= 12
} Class_Type;

typedef enum {
    FALSE = 0,
    TRUE  = 1
} Bool;

/*
 * Struttura Dato
 */

typedef struct {
	//Time time_stamp; // particolarizzare con la struttura dati fornita
	int16_t  value;
} Data;


/*
 * Struttura che descrive i sensori
 */
typedef struct {
	uint8_t 	  		ID;
	Class_Type 	  		sensor_class;
	Sensor_Link 	  	link;
	uint8_t		  		address;	 // solo per i2c, forse in futuro anche per analog
	uint32_t	  		period_ms;
	Bool 				dataread;
	Data 		  		lastData;
} Sensor;



// ATTENZIONE
/*anche l'msp è roba mia e la registrazione delle callback */


/* Metodi publici -----------------------------------------------------------*/

OP_STATE	initSensors 		(void);						//TEST
OP_STATE   	readData 			(uint8_t ID,Data *data);	//TEST

uint16_t	getSensorNumber 	();							//TEST
void 		getIDSensorList		(uint8_t *ID_array);
OP_STATE 	getClass			(uint8_t ID,Class_Type *class_type);	//TEST
OP_STATE	getPeriod 			(uint8_t ID,uint32_t * period);			//TEST

void 		destroyGSI	(); //DA FARE





#endif /* GSI_GSI_H_ */
