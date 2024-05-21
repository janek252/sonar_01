/*
 * stepper.h
 *
 *  Created on: 10 mar 2021
 *      Author: piotr
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#define STEPPER_ANGLE_MODE_SLAVE_TIM	1
#define STEPPER_ANGLE_MODE_MANUAL		2
#define STEPPER_ANGLE_MODE				STEPPER_ANGLE_MODE_MANUAL

#define STEPPER_MOTOR_MAX_FREQ_HZ	(MICRO_STEP * 1000)
#define STEPPER_MOTOR_MIN_FREQ_HZ	1

#define STEPPER_MOTOR_MAX_SPEED		100

#define STEP_PER_REVOLUTION			200
#define MICRO_STEP					32

typedef enum
{
	idle = 0,
	angle = 1,
	continous = 2
}stepper_mode;

typedef enum
{
	CCW = 0,
	CW = 1
}direction;

struct htim_s
{
	TIM_HandleTypeDef *htim;
	uint32_t channel;
};

struct stepper_s
{
	struct htim_s timer;
#if STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_SLAVE_TIM
	struct htim_s slave_timer;
#endif

	stepper_mode mode;
	volatile uint32_t step_counter;
	uint32_t steps_to_count;
};

#if STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_SLAVE_TIM
void stepper_init(struct stepper_s *, TIM_HandleTypeDef *, uint32_t , TIM_HandleTypeDef *);
#elif STEPPER_ANGLE_MODE == STEPPER_ANGLE_MODE_MANUAL
void stepper_init(struct stepper_s *, TIM_HandleTypeDef *, uint32_t);
#endif

void stepper_set_continous(struct stepper_s *, direction, uint32_t);
void stepper_set_angle(struct stepper_s *, direction, uint32_t, uint32_t);
void stepper_stop(struct stepper_s *);
void stepper_set_direction(struct stepper_s *, direction);
void stepper_set_speed(struct stepper_s *, uint32_t);

#endif /* INC_STEPPER_H_ */
