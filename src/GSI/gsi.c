/*
 * gsi.c
 *
 *  Created on: 04/lug/2015
 *      Author: ntonjeta
 */

#include "gsi_config.h"
#include "gsi.h"

/* Private variables ---------------------------------------------------------*/
Sensor sensorlist[SENSOR_NUMBER];

/* Peripheral Handle varibles ------------------------------------------------*/
I2C_HandleTypeDef hi2c3;
SPI_HandleTypeDef hspi2;
ADC_HandleTypeDef hadc1;

// RX Buffer I2C SPI ---------------------------------------------------------*/
int16_t 	*i2c_rx_buffer,*spi_rx_buffer;
// I2C/SPI variable ----------------------------------------------------------*/
int16_t 	spi_dato,i2c_dato;
uint16_t	spi_addres,i2c_addres;
// Analog sensor variable ----------------------------------------------------*/
uint32_t 	analog_data = 0,temp =0,tens;

/* Private function prototypes -----------------------------------------------*/
/* Initialization private function -------------------------------------------*/
void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI2_Init(void);
static void MX_ADC1_Init(void);
/* Logical private function --------------------------------------------------*/
Bool 	 isValidID (uint8_t ID,uint16_t *index);
//OP_STATE getSensorID (uint8_t ID,Sensor *sensor);




//Prevedere delle funzioni che fanno il controllo iniziale della lista

/*###############################################################################*/
/* ------------- Private logical function implementation ------------------------*/
/*###############################################################################*/

Bool isValidID (uint8_t ID,uint16_t *index)
{
	uint16_t i = 0;

	for(i=0;i<SENSOR_NUMBER;i++)
		if(ID == sensorlist[i].ID){
			*index = i;
			return TRUE;
		}
	return FALSE;
}


////ATTENZIONE SU QUESTA FUZNZIONE NUTRO FORTI DUBBI CHE FUNZIONERA'
////DA VEDERE BENE DEBUG
//OP_STATE getSensorID (uint8_t ID,Uint16_t sensorID)
//{
//	uint16_t i = 0;
//
//	if(isValidID(ID,&i) == TRUE){
//		sensor = &sensorlist[i];
//		return OP_OK;
//	}else return OP_ERR_ID;
//}



/*###############################################################################*/
/*###############################################################################*/


/*###############################################################################*/
/* ---------------------------- Method implementation ---------------------------*/
/*###############################################################################*/

OP_STATE initSensors (void)
{
/* Initialize hardware --------------------------------------------------------- */

#ifdef I2C_SENSOR_ENABLED
	MX_GPIO_Init();
	MX_I2C3_Init();
	i2c_rx_buffer = malloc(sizeof(uint16_t)*2);

#endif

#ifdef SPI_SENSOR_ENABLED
	MX_GPIO_Init();
	MX_SPI2_Init();
	spi_rx_buffer = malloc(sizeof(uint16_t));


#endif

#ifdef ADC_SENSOR_ENABLED
	MX_GPIO_Init();
	MX_ADC1_Init();
#endif


//	uint16_t i = 0;
//	for(i=0;i<SENSOR_NUMBER;i++){
////		sensorlist[i].ID 			= conf_ID[i];
////		sensorlist[i].sensor_class  = conf_sensor_class[i];
////		sensorlist[i].link 			= conf_link[i];
////		sensorlist[i].address 		= conf_addres[i];
////		sensorlist[i].period_ms		= conf_period[i];
////		sensorlist[i].dataread		= FALSE;

	sensorlist[0].ID 			= ID_1;
	sensorlist[0].sensor_class 	= CLASS_1;
	sensorlist[0].link			= LINK_1;
	sensorlist[0].address		= ADDRESS_1;
	sensorlist[0].period_ms		= PERIOD_1;
	sensorlist[0].dataread		= FALSE;


	sensorlist[1].ID 			= ID_2;
	sensorlist[1].sensor_class 	= CLASS_2;
	sensorlist[1].link			= LINK_2;
	sensorlist[1].address		= ADDRESS_2;
	sensorlist[1].period_ms		= PERIOD_2;
	sensorlist[1].dataread		= FALSE;


	sensorlist[2].ID 			= ID_3;
	sensorlist[2].sensor_class 	= CLASS_3;
	sensorlist[2].link			= LINK_3;
	sensorlist[2].address		= ADDRESS_3;
	sensorlist[2].period_ms		= PERIOD_3;
	sensorlist[2].dataread		= FALSE;


#ifdef I2C_SENSOR_ENABLED
	if(HAL_I2C_Slave_Receive_IT(&hi2c3,i2c_rx_buffer,sizeof(uint16_t)) != HAL_OK)
	{
		errorHandler();
	}
#endif

#ifdef SPI_SENSOR_ENABLED
	if(HAL_SPI_Receive_IT(&hspi2,spi_rx_buffer,sizeof(uint16_t)) != HAL_OK)
	{
		errorHandler();
	}

#endif

	return OP_OK;
}

OP_STATE readData (uint8_t ID,Data *data)
{
	uint16_t i = 0;
	uint16_t sensorID;

	if(isValidID(ID,&sensorID) != TRUE){
		return OP_ERR_ID;
	}

	//In tutto questo io ho ancora il problema di CATE da risolverema vedro di integrarlo con questo codice
	if(sensorlist[sensorID].link == I2C || sensorlist[sensorID].link == SPI){//SE si tratta di sensori digitali sincroni
		if(sensorlist[sensorID].dataread == TRUE){
			return OP_ERR_OVERUN;
		}else{
			data->value = sensorlist[sensorID].lastData.value;
		}
	}else if(sensorlist[sensorID].link == Analog){

			if(HAL_ADC_Start_IT(&hadc1) != HAL_OK)
			{
				errorHandler();//In questo caso dovrei aggiungere la gestione degli errori
			}else{
				//while(sensorlist[sensorID].dataread == FALSE) __NOP(); //Faccio una cosa un pò bloccante ma il DMA???
				sensorlist[sensorID].dataread = FALSE;
			}
	}else if(sensorlist[sensorID].link == CATE){
		//To future implementation
	}

	return OP_OK;

}

/*###############################################################################*/
/*------------------------------- GET Method ------------------------------------*/
/*###############################################################################*/


void getIDSensorList(uint8_t *ID_array)
{
	uint16_t i = 0;
	for(i=0;i<SENSOR_NUMBER;i++)
	{
		ID_array[i]= sensorlist[i].ID;
	}
}

uint16_t getSensorNumber ()
{
	return (uint16_t)SENSOR_NUMBER;
}

OP_STATE getClass (uint8_t ID,Class_Type *class_type)
{
	uint16_t i = 0;
	if(isValidID(ID,&i) == TRUE){
		*class_type = sensorlist[i].sensor_class;
		return OP_OK;
	}
	return OP_ERR_ID;
}

OP_STATE getPeriod (uint8_t ID,uint32_t * period)
{
	uint16_t i;
	if(isValidID(ID,&i)){
		*period = sensorlist[i].period_ms;
		return OP_OK;
	}
	return OP_ERR_ID;
}

/*###############################################################################*/
/*###############################################################################*/

/*###############################################################################*/
/* -------------------- CallBack Implementation ---------------------------------*/
/*###############################################################################*/

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	int16_t i = 0;

	spi_dato = *spi_rx_buffer;

	for(i=0;i<SENSOR_NUMBER;i++)
	{
		if(sensorlist[i].link == SPI){
			sensorlist[i].lastData.value = spi_dato;
			sensorlist[i].dataread = FALSE;
			break;
		}
	}

	//BSP_LED_Toggle(LED4);
	if(HAL_SPI_Receive_IT(&hspi2,spi_rx_buffer,sizeof(uint16_t)) != HAL_OK)
	{
		//qui va la logica applicattiva di gestione degli
		errorHandler();
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	int16_t i = 0;

	i2c_addres	= i2c_rx_buffer[0];
	i2c_dato	= i2c_rx_buffer[1];
	for(i=0;i<SENSOR_NUMBER;i++)
	{
		if(sensorlist[i].link == I2C && sensorlist[i].address == i2c_addres){
			sensorlist[i].lastData.value = i2c_dato;
			sensorlist[i].dataread = FALSE;
			break;
		}
	}

	//BSP_LED_Toggle(LED3);
	if(HAL_I2C_Slave_Receive_IT(&hi2c,i2c_rx_buffer,sizeof(uint16_t)*2) != HAL_OK)
	{
		errorHandler();
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
}

//Per ora la inseriamo ma forse questa callback si deve togliere
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	int16_t i = 0;

	//BSP_LED_Toggle(LED6);
	analog_data=HAL_ADC_GetValue(hadc);
	analog_data-= Calibration_Value;

	for(i=0;i<SENSOR_NUMBER;i++)
	{
		if(sensorlist[i].link == Analog){
			sensorlist[i].lastData.value = analog_data;
			sensorlist[i].dataread = TRUE;
			break;
		}
	}
}


/*###############################################################################*/
/*###############################################################################*/

/*###############################################################################*/
/*------------------  Initialization function implementation --------------------*/
/*###############################################################################*/

#ifdef I2C_SENSOR_ENABLED

/* I2C3 init function */
void MX_I2C3_Init(void)
{

  hi2c3.Instance 				= I2C3;
  hi2c3.Init.ClockSpeed 		= 100000;
  hi2c3.Init.DutyCycle 			= I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 		= 0;
  hi2c3.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode	= I2C_DUALADDRESS_DISABLED;
  hi2c3.Init.OwnAddress2 		= 0;
  hi2c3.Init.GeneralCallMode	= I2C_GENERALCALL_DISABLED;
  hi2c3.Init.NoStretchMode 		= I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c3);

}



#endif

#ifdef SPI_SENSOR_ENABLED

/* SPI1 init function */
void MX_SPI2_Init(void)
{

  hspi2.Instance 				= SPI2;
  hspi2.Init.Mode 				= SPI_MODE_SLAVE;
  hspi2.Init.Direction 			= SPI_DIRECTION_1LINE;
  hspi2.Init.DataSize 			= SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity 		= SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase 			= SPI_PHASE_1EDGE;
  hspi2.Init.NSS 				= SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit 			= SPI_FIRSTBIT_LSB;
  hspi2.Init.TIMode 			= SPI_TIMODE_DISABLED;
  hspi2.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
  hspi2.Init.CRCPolynomial		= 0;
  HAL_SPI_Init(&hspi2);

}

#endif

#ifdef ADC_SENSOR_ENABLED

/*ADC Initialization */
void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	hadc1.Instance 						= ADC1;
	hadc1.Init.ClockPrescaler 			= ADC_CLOCKPRESCALER_PCLK_DIV8;
	hadc1.Init.Resolution 				= ADC_RESOLUTION12b;
	hadc1.Init.ScanConvMode 			= DISABLE;
	hadc1.Init.ContinuousConvMode 		= DISABLE;
	hadc1.Init.DiscontinuousConvMode 	= DISABLE;
	hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion 			= 1;
	hadc1.Init.DMAContinuousRequests 	= DISABLE;
	hadc1.Init.EOCSelection 			= EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	*/
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}


#endif

void MX_GPIO_Init()
{
  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
}



/*###############################################################################*/
/*###############################################################################*/

/*###############################################################################*/
/*----.------------------- MSP Initialization Function --------------------------*/
/*###############################################################################*/

#ifdef I2C_SENSOR_ENABLED

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C3)
  {

    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    GPIO_InitStruct.Pin 		= GPIO_PIN_9;
    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
    GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate 	= GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin 		= GPIO_PIN_8;
    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
    GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate 	= GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __I2C3_CLK_ENABLE();
   /* System interrupt init*/
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
  }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if(hi2c->Instance==I2C3)
  {
    /* Peripheral clock disable */
    __I2C3_CLK_DISABLE();

    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);

    HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
  }

}

#endif

#ifdef SPI_SENSOR_ENABLED

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI2)
  {
     /* Peripheral clock enable */
    __SPI2_CLK_ENABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI1_SCK
    PB14     ------> SPI1_MISO
    PB15     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* System interrupt init*/
    HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }

}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI2)
  {
	/* Peripheral clock disable */
    __SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI1_SCK
    PB14     ------> SPI1_MISO
    PB15     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(SPI2_IRQn);
  }

}

#endif

#ifdef ADC_SENSOR_ENABLED

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
      /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* System interrupt init*/
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
     /* Peripheral clock disable */
    __ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  }

}

#endif

/*###############################################################################*/
/*###############################################################################*/


/*###############################################################################*/
/*------------------------- CallBack Registration -------------------------------*/
/*###############################################################################*/
#ifdef I2C_SENSOR_ENABLED

void I2C3_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c3);
}

/**
* @brief This function handles I2C1 event interrupt.
*/
void I2C3_EV_IRQHandler(void)
{
   HAL_I2C_EV_IRQHandler(&hi2c3);
}

#endif

#ifdef SPI_SENSOR_ENABLED

void SPI2_IRQHandler	(void)
{
	HAL_SPI_IRQHandler(&hspi2);

}

#endif

#ifdef ADC_SENSOR_ENABLED

void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
}

#endif

/*###############################################################################*/
/*###############################################################################*/



//RICORDARSI DI REGISTRARE LE CALLBACK forse è logica applicativa mo vediamo


void 		destroyGSI	()
{
	//DA FARE
}


//FUNZIONI DI COMODO DA ANDARE A CAMBIARE DOPO


void errorHandler(void)
{
	//BSP_LED_Toggle(LED5);
	while(1) {__NOP();}
}
