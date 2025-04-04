#include "motor.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <config.h>

// External declarations for timer handle
extern TIM_HandleTypeDef htim2;

// Test sequence completion flag (declare in main.c)
extern uint8_t motor_test_complete;

void reset_motor(){
	char msg[2];
	while(1){
		receive_msg(msg);

		if(msg[0] == 's'){
			set_motor_direction(-7, 0);
//			HAL_Delay(100);
//			set_motor_direction(0, 0);
//			msg[0] = 'g';
		}
		else if(msg[0] == 'w'){
			set_motor_direction(7, 0);
//			HAL_Delay(100);
//			set_motor_direction(0, 0);
//			msg[0] = 'g';

		}
		else if(msg[0] == 'd'){
			set_motor_direction(0, 7);
//			HAL_Delay(100);
//			set_motor_direction(0, 0);
//			msg[0] = 'g';
		}
		else if(msg[0] == 'a'){
			set_motor_direction(0, -7);
//			HAL_Delay(100);
//			set_motor_direction(0, 0);
//			msg[0] = 'g';
		}
		else if(msg[0] == 'k'){
					set_motor_direction(0, 0);
					msg[0] = 'g';
				}
		else if(msg[0] == 'q'){
			msg[0] = 'g';
			HAL_Delay(1000);
			return;
		}
	}


}


static uint16_t convert_to_speed(int input)
{
    const int input_max = 5;
    int abs_input = abs(input);
    uint16_t speed = (uint16_t)((abs_input * PWM_MAX) / input_max);

    if (speed < PWM_MIN) speed = PWM_MIN;
    else if (speed > PWM_MAX) speed = PWM_MAX;


    return speed;
}

void set_motor_direction(int Dx, int Dy)
{

	uint16_t x_speed = convert_to_speed(Dx);
	uint16_t y_speed = convert_to_speed(Dy);

	x_speed -= 150;

    // Control X-axis motor (TIM1_CH2)
    if (Dx > 0) {
        // Clockwise
        HAL_GPIO_WritePin(MOTOR_X_IN1_PORT, MOTOR_X_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_X_IN2_PORT, MOTOR_X_IN2_PIN, GPIO_PIN_RESET);
    } else if (Dx < 0) {
        // Counter-Clockwise
        HAL_GPIO_WritePin(MOTOR_X_IN1_PORT, MOTOR_X_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_X_IN2_PORT, MOTOR_X_IN2_PIN, GPIO_PIN_SET);
    } else {
        // Stop
        HAL_GPIO_WritePin(MOTOR_X_IN1_PORT, MOTOR_X_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_X_IN2_PORT, MOTOR_X_IN2_PIN, GPIO_PIN_RESET);
    }

    // Control Y-axis motor (TIM1_CH3)
    if (Dy > 0) {
        // Clockwise
        HAL_GPIO_WritePin(MOTOR_Y_IN1_PORT, MOTOR_Y_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_Y_IN2_PORT, MOTOR_Y_IN2_PIN, GPIO_PIN_RESET);

    } else if (Dy < 0) {

    	y_speed -= 100;
        // Counter-Clockwise
        HAL_GPIO_WritePin(MOTOR_Y_IN1_PORT, MOTOR_Y_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_Y_IN2_PORT, MOTOR_Y_IN2_PIN, GPIO_PIN_SET);
    } else {
        // Stop
        HAL_GPIO_WritePin(MOTOR_Y_IN1_PORT, MOTOR_Y_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_Y_IN2_PORT, MOTOR_Y_IN2_PIN, GPIO_PIN_RESET);
    }

    // Set PWM duty cycles
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_X, x_speed);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_Y, y_speed);
}

void run_motor_test_sequence(void)
{
    // Test 1: X-axis forward ramp
    for (int speed = 0; speed <= 100; speed += 10) {
        set_motor_direction(speed, 0);
        HAL_Delay(200);
    }
    HAL_Delay(100);

    // Test 2: X-axis reverse ramp
    for (int speed = 0; speed >= -100; speed -= 10) {
        set_motor_direction(speed, 0);
        HAL_Delay(200);
    }
    HAL_Delay(100);

    // Test 3: Y-axis forward ramp
    for (int speed = 0; speed <= 100; speed += 10) {
        set_motor_direction(0, speed);
        HAL_Delay(200);
    }
    HAL_Delay(100);

    // Test 4: Y-axis reverse ramp
    for (int speed = 0; speed >= -100; speed -= 10) {
        set_motor_direction(0, speed);
        HAL_Delay(200);
    }
    HAL_Delay(100);

    // Test 5: Both motors diagonal
    set_motor_direction(80, 80);
    HAL_Delay(200);
    set_motor_direction(-80, -80);
    HAL_Delay(200);

    // Test 6: Stop all motors
    set_motor_direction(0, 0);
    motor_test_complete = 1;
}
