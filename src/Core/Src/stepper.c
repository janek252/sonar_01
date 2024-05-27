/*
 * stepper.c
 *
 *  Created on: 10 mar 2021
 *      Author: piotr
 */

#include "main.h"

#if STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_SLAVE_TIM
void stepper_init(struct stepper_s *_stepper, TIM_HandleTypeDef *_htim, uint32_t _channel, TIM_HandleTypeDef *_slave_timer)
{
	_stepper->timer.htim = _htim;
	_stepper->timer.channel = _channel;
	_stepper->slave_timer.htim = _slave_timer;
}
#elif STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_MANUAL
void stepper_init(struct stepper_s *_stepper, TIM_HandleTypeDef *_htim, uint32_t _channel)
{
	_stepper->timer.htim = _htim;
	_stepper->timer.channel = _channel;
}
#endif

void stepper_set_continous(struct stepper_s *_stepper, direction _dir, uint32_t _speed)
{
	_stepper->mode = continous;

	stepper_set_direction(_stepper, _dir);
	stepper_set_speed(_stepper, _speed);

	HAL_TIM_PWM_Start(_stepper->timer.htim, _stepper->timer.channel);
}

void stepper_set_angle(struct stepper_s *_stepper, direction _dir, uint32_t _speed, uint32_t _angle)
{
	_stepper->mode = angle;

	stepper_set_direction(_stepper, _dir);
	stepper_set_speed(_stepper, _speed);

	_stepper->step_counter = 0;
	_stepper->steps_to_count = _angle * (STEP_PER_REVOLUTION * MICRO_STEP) / 360;

	if(0 == _stepper->steps_to_count)
	{
		stepper_stop(_stepper);
	}

#if STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_SLAVE_TIM
	__HAL_TIM_SET_COUNTER(_stepper->slave_timer.htim, 0);
	__HAL_TIM_SET_AUTORELOAD(_stepper->slave_timer.htim, _stepper->steps_to_count - 1);
	HAL_TIM_Base_Start_IT(_stepper->slave_timer.htim);

	HAL_TIM_PWM_Start(_stepper->timer.htim, _stepper->timer.channel);
#elif STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_MANUAL
	HAL_TIM_PWM_Start_IT(_stepper->timer.htim, _stepper->timer.channel);
#endif
}

void stepper_stop(struct stepper_s *_stepper)
{
	_stepper->mode = idle;

	__HAL_TIM_SET_COMPARE(_stepper->timer.htim, _stepper->timer.channel, 0);
	HAL_TIM_PWM_Stop(_stepper->timer.htim, _stepper->timer.channel);

#if STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_SLAVE_TIM
	HAL_TIM_Base_Stop_IT(_stepper->slave_timer.htim);
#endif
}

void stepper_set_direction(struct stepper_s *_stepper, direction _dir)
{
	if(_dir == CCW)
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, CCW);
	else if(_dir == CW)
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, CW);
}

void stepper_set_speed(struct stepper_s *_stepper, uint32_t _speed)
{
	uint32_t counter, freq;

	if(_speed > 100)
	{
		_speed = 100;
	}
	else if(_speed == 0)
	{
		stepper_stop(_stepper);
		return;
	}

	freq = (_speed * (STEPPER_MOTOR_MAX_FREQ_HZ - STEPPER_MOTOR_MIN_FREQ_HZ))/STEPPER_MOTOR_MAX_SPEED;
	counter = HAL_RCC_GetPCLK1Freq() / (_stepper->timer.htim->Init.Prescaler * freq);

	__HAL_TIM_SET_COUNTER(_stepper->timer.htim, 0);
	__HAL_TIM_SET_AUTORELOAD(_stepper->timer.htim, counter - 1);
	__HAL_TIM_SET_COMPARE(_stepper->timer.htim, _stepper->timer.channel, (counter/2) - 1);
}
