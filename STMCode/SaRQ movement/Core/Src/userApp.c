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
uint16_t servo(uint16_t anglePassed);
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
	//Task to pass angle for servo with a simple loop
	uint16_t anglePassed = 0;
	uint32_t CCR_Return=0;
	while(1) {
		if(anglePassed >= 180){
			anglePassed = 0;
		}
		else{
			anglePassed += 10;
		}
		//Get CCR so that each task can assaign based on its own CCR and timer
		CCR_Return = servo(anglePassed);
		TIM2->CCR1 = CCR_Return;
		printf("CCR1 is %lu\r\n\n", CCR_Return);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

uint16_t servo(uint16_t anglePassed){
	//Servo function calculated CCR value and returns it
	//Remove CCR_Val after testing complete
	uint16_t Min_ARR= 138, Max_ARR=1044, AngleRange=180;
	return ((Max_ARR-Min_ARR)*anglePassed)/AngleRange + Min_ARR;
}



