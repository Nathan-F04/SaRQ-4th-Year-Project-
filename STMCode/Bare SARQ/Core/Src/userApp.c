/*
 * userApp.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Niall.OKeeffe@atu.ie
 */

#include "userApp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stm32l4xx_hal.h"
//--------------------------------------------------------------
//used for real time stats, do not delete code from this section
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;

#define CCReg1 1
#define CCReg2 2
#define CCReg3 3
#define CCReg4 4

extern volatile unsigned long ulHighFrequencyTimerTicks;
void configureTimerForRunTimeStats(void) {
	ulHighFrequencyTimerTicks = 0;
	HAL_TIM_Base_Start_IT(&htim7);
}

unsigned long getRunTimeCounterValue(void) {
	return ulHighFrequencyTimerTicks;
}
//end of real time stats code
//----------------------------------------------------------------

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
// _write function used for printf
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}

//uint16_t servo(uint16_t anglePassed);
void servo(float anglePassed, uint8_t num, TIM_HandleTypeDef *htim);
//uint16_t servo(uint16_t anglePassed);

void userApp() {
	printf("Starting application, standing the SaRQ up\r\n\n");
	printf("\r\n");
	char rx_buffer[1]; //number + carraige return and newline
	uint8_t msg = 0;

	//Messages for the RPI
	char tx_buffer[] = "Command executed\r\n";
	char tx_err_buffer[] = "Invalid number sent\r\n";
	char tx_start_buffer[] = "Starting the RPI\r\n";

	//pwm starts here and ccrms etc
	//TIM 2
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

	//TIM 3
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	//TIM 4
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	//TIM 5
	TIM5->CCR1 = 0;
	TIM5->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

	//TIM 16
	TIM16->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	TIM2->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	//Starting the RPI
	HAL_UART_Transmit(&huart3, (uint8_t*)tx_start_buffer, strlen(tx_start_buffer), HAL_MAX_DELAY);

	//STANDING ANIMATION

	//REAR RIGHT
	//Elbow
	servo(50, CCReg1, &htim16);
	HAL_Delay(200);
	//Shoulder -15
	servo(75, CCReg4, &htim4);
	HAL_Delay(200);
	//Hip
	servo(110, CCReg1, &htim2);
	HAL_Delay(200);

	//REAR LEFT
	//Elbow
	servo(50, CCReg1, &htim3);
	HAL_Delay(200);
	//Shoulder offset of -5
	servo(85, CCReg2, &htim3);
	HAL_Delay(200);
	//Hip
	servo(60, CCReg3, &htim2);
	HAL_Delay(200);

	//FRONT RIGHT
	//Elbow
	servo(60, CCReg4, &htim3);
	HAL_Delay(200);
	//Shoulder offset of -10
	servo(80, CCReg4, &htim5);
	HAL_Delay(200);
	//Hip -15 offset
	servo(75, CCReg3, &htim3);
	HAL_Delay(200);

	//FRONT LEFT
	//Hip
	servo(100, CCReg2, &htim2);
	//Shoulder
	servo(90, CCReg3, &htim4);
	//Elbow
	servo(60, CCReg1, &htim5);
	HAL_Delay(5000);

	while (1) {
		HAL_UART_Receive(&huart3, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
		printf("Received: %s\r\n", rx_buffer);
		//Read code from atoi
		msg = atoi(rx_buffer);
		//Reset before looping again
		memset(rx_buffer, 0, sizeof(rx_buffer));
		//Case statement to determine movement
		switch (msg) {
		//Forward
		case 1:
			printf("1 received, going forward\r\n");
			//Get CCR so that each task can assign based on its own CCR and timer
			//Set angle passed for each based on ik

			printf("Rear left leg up and forward\r\n");
			//REAR LEFT
			//Shoulder offset of -5
			servo(105, CCReg2, &htim3);
			//Elbow
			servo(70, CCReg1, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -5
			servo(85, CCReg2, &htim3);
			HAL_Delay(1000);

			printf("Front left leg up and forward\r\n");
			//FRONT LEFT
			//shoulder
			servo(105, CCReg3, &htim4);
			//Elbow
			servo(50, CCReg1, &htim5);
			HAL_Delay(200);
			//shoulder
			servo(90, CCReg3, &htim4);
			HAL_Delay(1000);

			printf("Rear right leg up and forward\r\n");
			//REAR RIGHT
			//Shoulder offset of -15
			servo(85, CCReg4, &htim4);
			//Elbow
			servo(65, CCReg1, &htim16);
			HAL_Delay(500);
			//Shoulder offset of -15 may remove this line
			servo(75, CCReg4, &htim4);
			HAL_Delay(1000);

			printf("Front right leg up and forward\r\n");
			//FRONT RIGHT
			//Shoulder offset of -10
			servo(90, CCReg4, &htim5);
			//Elbow
			servo(50, CCReg4, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -10
			servo(80, CCReg4, &htim5);

			/*
			 * MOVING THEM FORWARD TOGETHER
			 */
			printf("All legs step forward\r\n");
			//REAR LEFT
			//Elbow
			servo(50, CCReg1, &htim3);

			//FRONT LEFT
			//Elbow
			servo(60, CCReg1, &htim5);

			//REAR RIGHT
			//Elbow
			servo(50, CCReg1, &htim16);

			//FRONT RIGHT
			//Elbow
			servo(60, CCReg4, &htim3);

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Reverse
		case 2:
			printf("2 received, going in reverse\r\n");

			printf("Rear left leg up and reverse\r\n");
			//REAR LEFT
			//Shoulder offset of -5
			servo(105, CCReg2, &htim3);
			//Elbow
			servo(30, CCReg1, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -5
			servo(85, CCReg2, &htim3);
			HAL_Delay(1000);

			printf("Front left leg up and reverse\r\n");
			//FRONT LEFT
			//shoulder
			servo(100, CCReg3, &htim4);
			//Elbow
			servo(70, CCReg1, &htim5);
			HAL_Delay(200);
			//shoulder
			servo(90, CCReg3, &htim4);
			HAL_Delay(1000);

			printf("Rear right leg up and reverse\r\n");
			//REAR RIGHT
			//Shoulder offset of -15
			servo(75, CCReg4, &htim4);
			HAL_Delay(200);
			//Elbow
			servo(35, CCReg1, &htim16);
			HAL_Delay(500);
			//Shoulder offset of -15 may remove this line
			servo(75, CCReg4, &htim4);
			HAL_Delay(1000);

			printf("Front right leg up and reverse\r\n");
			//FRONT RIGHT
			//Shoulder offset of -10
			servo(90, CCReg4, &htim5);
			HAL_Delay(200);
			//Elbow
			servo(70, CCReg4, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -10
			servo(80, CCReg4, &htim5);

			/*
			 * MOVING THEM BACKWARDS TOGETHER
			 */
			printf("All legs reverse\r\n");
			//REAR LEFT
			//Elbow
			servo(50, CCReg1, &htim3);

			//FRONT LEFT
			//Elbow
			servo(60, CCReg1, &htim5);

			//REAR RIGHT
			//Elbow
			servo(50, CCReg1, &htim16);

			//FRONT RIGHT
			//Elbow
			servo(60, CCReg4, &htim3);

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Left
		case 3:
			printf("3 received, turning left\r\n");
			printf("Rear left leg up and left\r\n");
			//REAR LEFT
			//Shoulder offset of -5
			servo(105, CCReg2, &htim3);
			//Hip
			servo(80, CCReg3, &htim2);
			HAL_Delay(500);
			//Shoulder offset of -5
			servo(85, CCReg2, &htim3);
			HAL_Delay(1000);

			printf("Front left leg up and left\r\n");
			//FRONT LEFT
			//shoulder
			servo(105, CCReg3, &htim4);
			//Hip
			servo(120, CCReg2, &htim2);
			HAL_Delay(200);
			//shoulder
			servo(90, CCReg3, &htim4);
			HAL_Delay(1000);

			printf("Rear right leg up and left\r\n");
			//REAR RIGHT
			//Shoulder offset of -15
			servo(95, CCReg4, &htim4);
			HAL_Delay(200);
			//Hip
			servo(130, CCReg1, &htim2);
			HAL_Delay(500);
			//Shoulder offset of -15 may remove this line
			servo(75, CCReg4, &htim4);
			HAL_Delay(1000);

			printf("Front right leg up and left\r\n");
			//FRONT RIGHT
			//Shoulder offset of -10
			servo(90, CCReg4, &htim5);
			HAL_Delay(200);
			//Hip
			servo(95, CCReg3, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -10
			servo(80, CCReg4, &htim5);
			HAL_Delay(1000);


			/*
			 * MOVING THEM LEFT TOGETHER
			 */

			printf("All legs step left\r\n");
			//REAR LEFT
			//Hip
			servo(60, CCReg3, &htim2);

			//FRONT LEFT
			//Hip
			servo(100, CCReg2, &htim2);

			//REAR RIGHT
			//Hip
			servo(110, CCReg1, &htim2);

			//FRONT RIGHT
			//Hip
			servo(75, CCReg3, &htim3);

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Right
		case 4:
			printf("4 received, turning right\r\n");


			printf("Rear left leg up and right\r\n");
			//REAR LEFT
			//Shoulder offset of -5
			servo(105, CCReg2, &htim3);
			//Hip
			servo(40, CCReg3, &htim2);
			HAL_Delay(500);
			//Shoulder offset of -5
			servo(85, CCReg2, &htim3);
			HAL_Delay(1000);

			printf("Front left leg up and right\r\n");
			//FRONT LEFT
			//shoulder
			servo(105, CCReg3, &htim4);
			//Hip
			servo(80, CCReg2, &htim2);
			HAL_Delay(200);
			//shoulder
			servo(90, CCReg3, &htim4);
			HAL_Delay(1000);

			printf("Rear right leg up and right\r\n");
			//REAR RIGHT
			//Shoulder offset of -15
			servo(95, CCReg4, &htim4);
			HAL_Delay(200);
			//Hip
			servo(90, CCReg1, &htim2);
			HAL_Delay(500);
			//Shoulder offset of -15 may remove this line
			servo(75, CCReg4, &htim4);
			HAL_Delay(1000);

			printf("Front right leg up and right\r\n");
			//FRONT RIGHT
			//Shoulder offset of -10
			servo(90, CCReg4, &htim5);
			HAL_Delay(200);
			//Hip
			servo(55, CCReg3, &htim3);
			HAL_Delay(500);
			//Shoulder offset of -10
			servo(80, CCReg4, &htim5);
			HAL_Delay(1000);


			/*
			 * MOVING THEM LEFT TOGETHER
			 */

			printf("All legs step right\r\n");
			//REAR LEFT
			//Hip
			servo(60, CCReg3, &htim2);

			//FRONT LEFT
			//Hip
			servo(100, CCReg2, &htim2);

			//REAR RIGHT
			//Hip
			servo(110, CCReg1, &htim2);

			//FRONT RIGHT
			//Hip
			servo(75, CCReg3, &htim3);

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
		default:
			printf("Invalid number received, sending a message to RPI\r\n");

			//Send a command incomplete message to the RPI
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_err_buffer, strlen(tx_err_buffer), HAL_MAX_DELAY);
			break;
		}
	}
}

void servo(float anglePassed, uint8_t channel, TIM_HandleTypeDef *htim) {
	//Servo function calculated CCR value and returns it, angle is 210 as it is the servos max.
	float CCR_Return = 0;
	uint16_t Min_CCR = 1120, Max_CCR = 8495, AngleRange = 210;
	CCR_Return = ((Max_CCR - Min_CCR) * anglePassed) / AngleRange + Min_CCR;
	CCR_Return = roundf(CCR_Return);
	if (htim->Instance == TIM2) {
		switch (channel) {
		case 1:
			TIM2->CCR1 = CCR_Return;
			break;
		case 2:
			TIM2->CCR2 = CCR_Return;
			break;
		case 3:
			TIM2->CCR3 = CCR_Return;
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM3) {
		switch (channel) {
		case 1:
			TIM3->CCR1 = CCR_Return;
			break;
		case 2:
			TIM3->CCR2 = CCR_Return;
			break;
		case 3:
			TIM3->CCR3 = CCR_Return;
			break;
		case 4:
			TIM3->CCR4 = CCR_Return;
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM4) {
		switch (channel) {
		case 3:
			TIM4->CCR3 = CCR_Return;
			break;
		case 4:
			TIM4->CCR4 = CCR_Return;
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM5) {
		switch (channel) {
		case 1:
			TIM5->CCR1 = CCR_Return;
			break;
		case 4:
			TIM5->CCR4 = CCR_Return;
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM16) {
		TIM16->CCR1 = CCR_Return;
	}
}
