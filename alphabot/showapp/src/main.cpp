#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <bsp/bsp.h>
#include <sys/cdefs.h>
#include <rtl/countof.h>
#include "Motor.hpp"
#include <coresrv/time/time_api.h>


#define GPIO_PIN_NUM    RTL_UINT32_C(28)
#define DELAY_S         2
#define HW_MODULE_NAME  "gpio0"
#define HW_MODULE_CFG   "raspberry_pi4b.default"
#define GPIO_HIGH_LEVEL 1
#define GPIO_LOW_LEVEL  0

#define GPIO_PIN_UART_TX 14
#define GPIO_PIN_UART_RX 15

#define GPIO_PIN_MOTOR_IN1 12
#define GPIO_PIN_MOTOR_IN2 13
#define GPIO_PIN_MOTOR_ENA 6
#define GPIO_PIN_MOTOR_IN3 20
#define GPIO_PIN_MOTOR_IN4 21
#define GPIO_PIN_MOTOR_ENB 26

int main(void)
{
    auto last_run = KnGetMSecSinceStart();
    Motor motor(GPIO_PIN_MOTOR_IN1, GPIO_PIN_MOTOR_IN2, GPIO_PIN_MOTOR_ENA, GPIO_PIN_MOTOR_IN3, GPIO_PIN_MOTOR_IN4, GPIO_PIN_MOTOR_ENB);
    motor.begin();
    while (true)
    {
        motor.run();
        if (KnGetMSecSinceStart() - last_run > 60000)
        {
            motor.do_instruction(STRAIGHT, 1000);
            last_run = KnGetMSecSinceStart();
        }
    }
    return EXIT_SUCCESS;

}
