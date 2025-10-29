/*
 * userApp.c
 *
 *  Created on: Oct 29, 2025
 *      Author: ferry
 */
#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

void userApp(){
	printf("Starting my app\r\n\n");
	while(1){

	}
}
