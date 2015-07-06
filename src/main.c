//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

//#include <stdio.h>
//#include "diag/Trace.h"
//#include "stm32f4xx_hal.h"
//#include "stm32f4_discovery.h"
#include "gsi.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//Modifica di prova

/* Private variables ---------------------------------------------------------*/
Data data;
uint8_t *ID_vector;
uint16_t number = 0;

/* Function definition -------------------------------------------------------*/
/* Initialization function ---------------------------------------------------*/



/* Logical function -----------------------------------------------------------*/
void setup();
uint8_t isLoop();
void errorHandler();
void finalize();


/* Main function --------------------------------------------------------------*/
int main(){
	setup();
	uint16_t i= 0,j = 0;
	while(isLoop()){
		if(readData	(ID_vector[(i%number)],&data) != OP_OK){

			errorHandler();
		}
		i++;
		HAL_Delay(1000);
	}

	//finalize();
}

/*#################################		Setup Block		########################*/

void setup(){
	/*Initialize hardware */

	if(initSensors() != OP_OK)
	{
		errorHandler();
	}

	number = getSensorNumber();

	ID_vector = (uint8_t *) malloc(sizeof(uint8_t)*number);

	getIDSensorList	(ID_vector);

	/*Initialize BSP */
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	BSP_LED_On(LED6);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	}



/*########################		Functional block	###########################*/

uint8_t isLoop()
{
	return 1;
}

void finalize()
{

}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
