

#ifndef MOTOR_H
#define MOTOR_H

#include <bsp/bsp.h>
#include <gpio/gpio.h>
#include <rtl/countof.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>

#define HW_MODULE_NAME "gpio0"
#define HW_MODULE_CFG "raspberry_pi4b.default"

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

  private:
    int pin1;
    int pin2;
    int enA;
    int pin3;
    int pin4;
    int enB;
    bool initialized;
    GpioHandle handle;

    void run(int time, int speed);
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

    fprintf(stderr, "Test finished.\n");
    this->initialized = true;
    this->handle = handle;
    return EXIT_SUCCESS;
}

void Motor::do_instruction(int type, int time, int speed)
{
    switch (type)
    {
    case STRAIGHT:
        this->run(time, speed);
        break;
    case TURN:
        this->turn(time, speed);
        break;
    default:
        break;
    }
}

void Motor::run(int time, int speed)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    GpioOut(this->handle, this->pin1, GPIO_VALUE_HIGH);
    GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
    GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin4, GPIO_VALUE_HIGH);
    GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);

    sleep(time * 1000);

    GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enA, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enB, GPIO_VALUE_LOW);
}

void Motor::turn(int time, int speed)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
    GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin4, GPIO_VALUE_HIGH);
    GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);

    sleep(time * 1000);

    GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enA, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
    GpioOut(this->handle, this->enB, GPIO_VALUE_LOW);
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
}

#endif
