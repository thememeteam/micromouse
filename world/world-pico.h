#include "../common.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define MOTOR_R_A 26
#define MOTOR_R_B 16
#define MOTOR_L_A 27
#define MOTOR_L_B 17

#define IR_LEFT 6
#define IR_FRONT 7
#define IR_RIGHT 8

void worldInit()
{
    // motors
    gpio_set_function(MOTOR_L_A, GPIO_FUNC_PWM);
    gpio_set_dir(MOTOR_L_A, GPIO_OUT);
    gpio_set_function(MOTOR_L_B, GPIO_FUNC_PWM);
    gpio_set_dir(MOTOR_L_B, GPIO_OUT);
    gpio_set_function(MOTOR_R_A, GPIO_FUNC_PWM);
    gpio_set_dir(MOTOR_R_A, GPIO_OUT);
    gpio_set_function(MOTOR_R_B, GPIO_FUNC_PWM);
    gpio_set_dir(MOTOR_R_B, GPIO_OUT);

    uint a_slice_num = pwm_gpio_to_slice_num(MOTOR_L_A);
    uint b_slice_num = pwm_gpio_to_slice_num(MOTOR_L_B);

    pwm_set_clkdiv(a_slice_num, 8.0f);
    pwm_set_clkdiv(b_slice_num, 8.0f);

    pwm_set_wrap(a_slice_num, 65535);
    pwm_set_gpio_level(MOTOR_L_A, 0);
    pwm_set_gpio_level(MOTOR_R_A, 0);
    pwm_set_enabled(a_slice_num, true);

    pwm_set_wrap(b_slice_num, 65535);
    pwm_set_gpio_level(MOTOR_L_B, 0);
    pwm_set_gpio_level(MOTOR_R_B, 0);
    pwm_set_enabled(b_slice_num, true);

    // IR
    gpio_init(IR_LEFT);
    gpio_set_dir(IR_LEFT, GPIO_OUT);
    gpio_init(IR_FRONT);
    gpio_set_dir(IR_FRONT, GPIO_OUT);
    gpio_init(IR_RIGHT);
    gpio_set_dir(IR_RIGHT, GPIO_OUT);
}

Direction validSides()
{
    Direction res = DOWN;
    if(gpio_get(IR_LEFT))
        res |= LEFT;
    if(gpio_get(IR_FRONT))
        res |= UP;
    if(gpio_get(IR_RIGHT))
        res |= RIGHT;
    
    return res;
}

void moveToJunction(Direction dir)
{
    // turn
    switch (dir) {
        case LEFT:
            pwm_set_gpio_level(MOTOR_L_A, 0);
            pwm_set_gpio_level(MOTOR_R_A, 42000);
            // turn until the line on the other side is detected
            sleep_ms(50);
            while(!gpio_get(IR_FRONT)) { }
            pwm_set_gpio_level(MOTOR_R_A, 0);
            break;
        case RIGHT:
            pwm_set_gpio_level(MOTOR_R_A, 0);
            pwm_set_gpio_level(MOTOR_L_A, 42000);
            sleep_ms(50);
            while(!gpio_get(IR_FRONT)) { }
            pwm_set_gpio_level(MOTOR_L_A, 0);
            break;
        case DOWN:
            pwm_set_gpio_level(MOTOR_L_A, 42000); // reverse both directions
            pwm_set_gpio_level(MOTOR_L_B, 0);
            pwm_set_gpio_level(MOTOR_R_A, 0);
            pwm_set_gpio_level(MOTOR_R_B, 42000);
            sleep_ms(500); // wait so that it doesn't get stuck on a right path in between
            while(!gpio_get(IR_FRONT)) { }
            pwm_set_gpio_level(MOTOR_L_A, 0);
            pwm_set_gpio_level(MOTOR_L_B, 0);
            pwm_set_gpio_level(MOTOR_R_A, 0);
            pwm_set_gpio_level(MOTOR_R_B, 0);
            break;
    }

    // move straight until junction
    pwm_set_gpio_level(MOTOR_L_A, 42000);
    pwm_set_gpio_level(MOTOR_L_A, 42000);

    while(!gpio_get(IR_FRONT)) { }
    
    pwm_set_gpio_level(MOTOR_L_A, 0);
    pwm_set_gpio_level(MOTOR_R_A, 0);
}