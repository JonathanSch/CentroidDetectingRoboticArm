#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"
#include "main.h"

// Define motor control parameters
#define PWM_MIN 50
#define PWM_MAX 1000

void set_motor_direction(int Dx, int Dy);
void run_motor_test_sequence(void);
void test_motor_direction_simple(void);
void reset_motor(void);

#endif // MOTOR_H
