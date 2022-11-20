#ifndef MOTOR_H
#define MOTOR_H

#include <bsp/bsp.h>
#include <coresrv/time/time_api.h>
#include <gpio/gpio.h>
#include <rtl/countof.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <iostream>

#define HW_MODULE_NAME "gpio0"
#define HW_MODULE_CFG "raspberry_pi4b.default"

#define STOP 0
#define STOP 0
#define STRAIGHT 1
#define TURN 2

#define GPIO_VALUE_HIGH 1
#define GPIO_VALUE_LOW 0

class Motor
{
  public:
    Motor(int pin1, int pin2, int enA, int pin3, int pin4, int enB);
    int begin();
    void do_instruction(int type, int time, int speed);
    void stop();
    void run();
    bool is_stopped();

  private:
    int pin1;
    int pin2;
    int enA;
    int pin3;
    int pin4;
    int enB;
    bool initialized;
    bool stopped;
    GpioHandle handle;
    rtl_size_t stop_signal;

    void straight(int time, int speed);
    void turn(int time, int speed);
};

Motor::Motor(int pin1, int pin2, int enA, int pin3, int pin4, int enB)
{
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->enA = enA;
    this->pin3 = pin3;
    this->pin4 = pin4;
    this->enB = enB;
    this->initialized = false;
    this->stopped = false;
    this->handle = NULL;
    this->stop_signal = 0;
}

int Motor::begin()
{
    int returnValue = EXIT_FAILURE;
    Retcode rc = rcFail;
    GpioHandle handle = GPIO_INVALID_HANDLE;

    fprintf(stderr, "Start GPIO_output test.\n");

    /**
     * Initialize the board support package (BSP) driver and set configuration
     * for GPIO pins. It is required for stdout by UART.
     */
    rc = BspInit(NULL);
    if (rc != BSP_EOK)
    {
        fprintf(stderr, "Failed to initialize BSP, error code: %d.\n", RC_GET_CODE(rc));
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }

    rc = BspSetConfig(HW_MODULE_NAME, HW_MODULE_CFG);
    if (rc != rcOk)
    {
        fprintf(stderr,
                "Failed to set mux configuration for %s module, "
                "error code: %d.\n",
                HW_MODULE_NAME, RC_GET_CODE(rc));
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }

    /* Initialize the GPIO. */
    rc = GpioInit();
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioInit failed, error code: %d.\n", RC_GET_CODE(rc));
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }

    /* Initialize and setup HW_MODULE_NAME port. */
    rc = GpioOpenPort(HW_MODULE_NAME, &handle);
    if (rcOk != rc)
    {
        fprintf(stderr, "GpioOpenPort port %s failed, error code: %d.\n", HW_MODULE_NAME, RC_GET_CODE(rc));
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }
    else if (handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GPIO module %s handle is invalid.\n", HW_MODULE_NAME);
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }

    /* Set GPIO_PIN_NUM pin as output. */
    rc = 0;
    rc += GpioSetMode(handle, this->pin1, GPIO_DIR_OUT);
    rc += GpioSetMode(handle, this->pin2, GPIO_DIR_OUT);
    rc += GpioSetMode(handle, this->enA, GPIO_DIR_OUT);
    rc += GpioSetMode(handle, this->pin3, GPIO_DIR_OUT);
    rc += GpioSetMode(handle, this->pin4, GPIO_DIR_OUT);
    rc += GpioSetMode(handle, this->enB, GPIO_DIR_OUT);
    if (rcOk != rc)
    {
        fprintf(stderr,
                "GpioSetMode for module %s failed, "
                "error code: %d.\n",
                HW_MODULE_NAME, RC_GET_CODE(rc));
        fprintf(stderr, "Test failed.\n");
        return EXIT_FAILURE;
    }

    this->handle = handle;
    this->stop();
    this->stop_signal = KnGetMSecSinceStart();
    this->initialized = true;
    fprintf(stderr, "Test finished.\n");
    return EXIT_SUCCESS;
}

void Motor::do_instruction(int type, int time, int speed = 20)
{
    // std::cout << "do_instruction: " << type << " " << time << " " << speed << std::endl;
    fprintf(stderr, "do_instruction: %d %d %d\n", type, time, speed);
    switch (type)
    {
    case STOP:
        this->stop();
        break;
    case STRAIGHT:
        this->straight(time, speed);
        break;
    case TURN:
        this->turn(time, speed);
        break;
    default:
        break;
    }
}

void Motor::straight(int time, int speed)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    if (speed < 0) // move backward
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }
    else // move forward
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }

    this->stop_signal = KnGetMSecSinceStart() + time;
    this->stopped = false;
}

void Motor::turn(int time, int speed)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }
    if (speed > 0) // turn left
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }
    else // turn right
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }

    this->stop_signal = KnGetMSecSinceStart() + time;
    this->stopped = false;
}

void Motor::stop()
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enA, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enB, GPIO_VALUE_LOW);

    this->stopped = true;
}

void Motor::run()
{
    if (KnGetMSecSinceStart() > this->stop_signal && !this->stopped)
    {
        this->stop();
    }
}

bool Motor::is_stopped()
{
    return this->stopped;
}

#endif
