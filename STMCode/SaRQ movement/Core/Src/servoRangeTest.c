/*
 * userApp.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Niall.OKeeffe@atu.ie
 */

#include "userApp.h"
#include <stdio.h>

//Add the relevant FreeRTOS header files here
#include "FreeRTOS.h"
#include "task.h"

//--------------------------------------------------------------
//used for real time stats, do not delete code from this section
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;
extern volatile unsigned long ulHighFrequencyTimerTicks;
void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0;
    HAL_TIM_Base_Start_IT(&htim7);
}

unsigned long getRunTimeCounterValue(void)
{
	return ulHighFrequencyTimerTicks;
}
//end of real time stats code
//----------------------------------------------------------------

extern UART_HandleTypeDef huart1;
uint8_t flag = 0;
uint32_t pulse_length = 0;
static void timerTask(void * pvParameters);
// _write function used for printf
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}


void userApp() {
	printf("Starting application\r\n\n");

	xTaskCreate(timerTask, "Timer Task", 200, NULL, 2, NULL);
	TIM2->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	vTaskStartScheduler();

	while(1) {

	}
}

void timerTask(void * pvParameters) {
	printf("Starting timer task\r\n\n");
	//Task to test range for servo testing
	while(1) {
		for( uint16_t i = 138; i<1044; i++){
			i+=10;
			printf("I is %lu\r\n\n", i);
			vTaskDelay(pdMS_TO_TICKS(100));
		}
	}
}

