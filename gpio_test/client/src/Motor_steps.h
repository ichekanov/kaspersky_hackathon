

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

#define HW_MODULE_NAME "gpio0"
#define HW_MODULE_CFG "raspberry_pi4b.default"

#define STRAIGHT 1
#define TURN 2

#define GPIO_VALUE_HIGH 1
#define GPIO_VALUE_LOW 0

// 20 прорезей
#define STEPS_PER_CM 1
#define STEPS_PER_DEGREE 9/90

#define GPIO_EVENT_DELAY_MS 10

#define GPIO_PIN_STEP_R 8
#define GPIO_PIN_STEP_L 7

#define GPIO_INTERRUPT static_cast<GpioMode>(GPIO_DIR_IN|GPIO_EVENT_RISE_EDGE)

static rtl_uint32_t pinsBitmap = RTL_UINT32_C(0);

class Motor
{
  public:
    Motor(int pin1, int pin2, int enA, int pin3, int pin4, int enB);
    int begin();
    void do_instruction(int type, int value);
    void stop();
    void run();

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
    unsigned int number_of_steps;
    unsigned int current_steps;

    void straight(int distance);
    void turn(int angle);
    void count_step();
    void check_interrupt();
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
    this->number_of_steps = 0;
    this->current_steps = 0;
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
    rc += GpioSetMode(handle, GPIO_PIN_STEP_R, GPIO_INTERRUPT);
    // rc += GpioSetMode(handle, GPIO_PIN_STEP_L, GPIO_EVENT_RISE_EDGE);
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
    this->initialized = true;
    fprintf(stderr, "Test finished.\n");
    return EXIT_SUCCESS;
}

void Motor::do_instruction(int type, int value)
{
    switch (type)
    {
    case STRAIGHT:
        this->straight(value);
        break;
    case TURN:
        this->turn(value);
        break;
    default:
        break;
    }
}

void Motor::straight(int value)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    if (value < 0) // move forward
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }
    else // move backward
    {
        GpioOut(this->handle, this->pin1, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->pin2, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->enA, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin3, GPIO_VALUE_HIGH);
        GpioOut(this->handle, this->pin4, GPIO_VALUE_LOW);
        GpioOut(this->handle, this->enB, GPIO_VALUE_HIGH);
    }

    this->current_steps = 0;
    this->number_of_steps = value * STEPS_PER_CM;
    this->stopped = false;
}

void Motor::turn(int value)
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }
    if (value > 0) // turn left
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

    this->current_steps = 0;
    this->number_of_steps = value * STEPS_PER_DEGREE;
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

    this->current_steps = 0;
    this->number_of_steps = 0;
    this->stopped = true;
}

void Motor::run()
{
    this->check_interrupt();
    if (this->current_steps >= this->number_of_steps && !this->stopped)
    {
        this->stop();
    }
}

static bool IsFirstInterrupt(const rtl_uint32_t pin)
{
    bool isFirstInterruptOnPin = false;

    if (0U == (pinsBitmap & (1U << pin)))
    {
        isFirstInterruptOnPin = true;
    }

    return isFirstInterruptOnPin;
}

static void SetPinBitmap(const rtl_uint32_t pin)
{
    pinsBitmap |= (1U << pin);
}

void Motor::check_interrupt()
{
    if (!this->initialized)
    {
        fprintf(stderr, "Not initialized.\n");
        return;
    }

    char buf[sizeof(KdfEvent) + sizeof(GpioEvent)] = {0};
    KdfEvent *event = (KdfEvent *)buf;

    if (GpioGetEvent(this->handle, event, GPIO_EVENT_DELAY_MS) == rcOk)
    {
        auto pin = *((rtl_uint32_t *)event->payload);
        if ((event->type == GPIO_EVENT_TYPE_EDGE_RISE) && (pin == GPIO_PIN_STEP_L || pin == GPIO_PIN_STEP_R) && (IsFirstInterrupt(pin)))
        {
            SetPinBitmap(pin);
            fprintf(stderr, "Interrupt on %s module %d pin: pin %d is HIGH!\n", HW_MODULE_NAME, pin, pin);
            GpioReleaseMode(this->handle, pin);
            this->current_steps++;
        }
    }
}

#endif
