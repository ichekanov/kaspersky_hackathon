#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <bsp/bsp.h>
#include <sys/cdefs.h>
#include <rtl/countof.h>

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

/**
 * ExceptionPinArr is an array of GPIO pin numbers that should be excluded
 * from the example. This may be necessary if some of the pins are already
 * used for other functions, for example, if the pins are used for UART
 * connections during debugging.
 */
static const rtl_uint32_t ExceptionPinArr[] = { GPIO_PIN_UART_TX,
                                                GPIO_PIN_UART_RX };

static bool IsExceptionPin(const rtl_uint32_t pin)
{
    bool isExceptionPin = false;

    for (size_t i = 0; i < rtl_countof(ExceptionPinArr); i++)
    {
        if (ExceptionPinArr[i] == pin)
        {
            isExceptionPin = true;
            break;
        }
    }

    return isExceptionPin;
}

int main(void)
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
        fprintf(stderr,
                "Failed to initialize BSP, error code: %d.\n",
                RC_GET_CODE(rc));
    }

    if (rcOk == rc)
    {
        rc = BspSetConfig(HW_MODULE_NAME, HW_MODULE_CFG);
        if (rcOk != rc)
        {
            fprintf(stderr, "Failed to set mux configuration for %s module, "
                    "error code: %d.\n", HW_MODULE_NAME,
                    RC_GET_CODE(rc));
        }
    }

    /* Initialize the GPIO. */
    if (rcOk == rc)
    {
        rc = GpioInit();
        if (rcOk != rc)
        {
            fprintf(stderr, "GpioInit failed, error code: %d.\n",
                    RC_GET_CODE(rc));
        }
    }

    /* Initialize and setup HW_MODULE_NAME port. */
    if (rcOk == rc)
    {
        rc = GpioOpenPort(HW_MODULE_NAME, &handle);
        if (rcOk != rc)
        {
            fprintf(stderr,
                    "GpioOpenPort port %s failed, error code: %d.\n",
                    HW_MODULE_NAME, RC_GET_CODE(rc));
        }
        else if (GPIO_INVALID_HANDLE == handle)
        {
            fprintf(stderr, "GPIO module %s handle is invalid.\n",
                    HW_MODULE_NAME);
            rc = rcFail;
        }
    }

    /* Set GPIO_PIN_NUM pin as output. */
    if (rcOk == rc)
    {
        for (rtl_uint32_t pinNum = 0; pinNum < GPIO_PIN_NUM; pinNum++)
        {
            if (false == IsExceptionPin(pinNum))
            {
                rc = GpioSetMode(handle, pinNum, GPIO_DIR_OUT);
                if (rcOk != rc)
                {
                    fprintf(stderr, "GpioSetMode for module %s pin %u failed, "
                            "error code: %d.\n", HW_MODULE_NAME, pinNum,
                            RC_GET_CODE(rc));
                    break;
                }
            }
        }
    }

    /* Set all the GPIO pins to GPIO_HIGH_LEVEL */
    if (rcOk == rc)
    {
        for (rtl_uint32_t pinNum = 0; pinNum < GPIO_PIN_NUM - 1; pinNum++)
        {
            if (false == IsExceptionPin(pinNum))
            {
                rc = GpioOut(handle, pinNum, GPIO_HIGH_LEVEL);
                if (rcOk != rc)
                {
                    fprintf(stderr, "GpioOut 1 for module %s pin %u failed, "
                            "error code: %d.\n", HW_MODULE_NAME, pinNum,
                            RC_GET_CODE(rc));
                    break;
                }
                else
                {
                    fprintf(stderr, "Module %s pin %u is set to 1.\n",
                            HW_MODULE_NAME, pinNum);
                    sleep(DELAY_S);

                    rc = GpioOut(handle, pinNum, GPIO_LOW_LEVEL);
                    if (rcOk != rc)
                    {
                        fprintf(stderr, "GpioOut 0 for module %s pin %u "
                                "failed, error code: %d.\n",
                                HW_MODULE_NAME, pinNum, RC_GET_CODE(rc));
                        break;
                    }

                    fprintf(stderr, "Module %s pin %u is set to 0.\n",
                            HW_MODULE_NAME, pinNum);
                }
            }
        }
    }

    /* Close GPIO port. */
    if (GPIO_INVALID_HANDLE != handle)
    {
        Retcode tempRc = GpioClosePort(handle);
        if (rcOk != tempRc)
        {
            fprintf(stderr,
                    "GpioClosePort for %s port failed, error code: %d.\n",
                    HW_MODULE_NAME, RC_GET_CODE(tempRc));
            rc = tempRc;
        }
    }

    if (rcOk == rc)
    {
        fprintf(stderr, "Test finished.\n");
        returnValue = EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test failed.\n");
    }

    return returnValue;
}
