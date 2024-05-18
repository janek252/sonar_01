#include "main.h"
#include "tim.h"
#include "stm32l4xx_hal.h"
#include "gpio.h"

void delay_us(uint16_t time) //funkcja opóźnienia czasowego
{
	__HAL_TIM_SET_COUNTER(&htim7,0); // załącznie timera
	while (__HAL_TIM_GET_COUNTER(&htim7)<time); // wyłaczenie timera po upływie zadanego czasu
}


uint32_t time1 = 0;
uint32_t time2 = 0;
uint32_t diff = 0;
uint32_t distance = 0;
uint32_t start;
uint32_t stop;

void Sensor_trigger() //funkcja wysyłania pulsu trigger
{
	HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, 1); // załączenie pulsu trigger
	delay_us(10); // oczekiwanie 10us
	HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, 0); // wyłączenie pulsu trigger
}

int Sensor_get_distance() // funckja pomiaru odległości
{
	Sensor_trigger();

	while( !(HAL_GPIO_ReadPin(Echo_GPIO_Port, Echo_Pin ))); // czekanie na stan wysoki
	time1 = __HAL_TIM_GET_COUNTER(&htim7); // czas dla początku impulsu echo

	while( HAL_GPIO_ReadPin(Echo_GPIO_Port, Echo_Pin )); // czekanie na stan niski
	time2 = __HAL_TIM_GET_COUNTER(&htim7);  // czas dla końca pusu echo

	diff = time2 - time1; // pomiar długości pulsu echo
	distance = diff/58; // pomiar odległości według wzoru producenta

	return distance;
}

