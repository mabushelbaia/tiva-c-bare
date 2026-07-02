/**
 * @file    main.c
 * @brief   Motor control demo for the TM4C123GH6PM (EK-TM4C123GXL)
 *
 * Cycles through forward, reverse, brake and coast states while printing
 * encoder telemetry over UART.
 */

#include "TM4C123.h"
#include "motors/qei.h"
#include "motors/motor_driver.h"
#include "printf.h"
#include "uart.h"

#define TICK_HZ 10U
#define PRINT_TICKS 5U
#define STATE_TICKS 30U

#define VEL_WINDOW_HZ 10U

static motor_driver_t drv = {
    .stby_port = GPIOE,
    .stby_pin = (1 << 1),
    .motor_a = {
        .in1_port = GPIOA,
        .in1_pin = (1 << 2),
        .in2_port = GPIOA,
        .in2_pin = (1 << 3),
        .pwm = PWM0,
        .gen = 0,
        .pwm_ch = 0,
        .period = 1999,
    },
    .motor_b = {
        .in1_port = GPIOE,
        .in1_pin = (1 << 3),
        .in2_port = GPIOE,
        .in2_pin = (1 << 2),
        .pwm = PWM0,
        .gen = 0,
        .pwm_ch = 1,
        .period = 1999,
    },
};

static volatile uint32_t ticks;

/**
 * @brief   SysTick interrupt handler
 *
 * Increments the tick counter at TICK_HZ rate.
 */
void SysTick_Handler(void)
{
    ticks++;
}

/**
 * @brief   Execute one demo state
 *
 * @param state  Demo state index (0-4, wraps at 5):
 *               0 = forward 100%,
 *               1 = forward 100% / reverse 100% (opposite),
 *               2 = reverse 70%,
 *               3 = brake,
 *               4+ = coast
 */
static void demo_step(uint32_t state)
{
    switch (state) {
    case 0:
        printf("\r\n>> forward 50%%\r\n");
        motor_channel_forward(&drv.motor_a, 100);
        motor_channel_forward(&drv.motor_b, 100);
        break;
    case 1:
        printf("\r\n>> forward 100%%\r\n");
        motor_channel_forward(&drv.motor_a, 100);
        motor_channel_reverse(&drv.motor_b, 100);
        break;
    case 2:
        printf("\r\n>> reverse 70%%\r\n");
        motor_channel_reverse(&drv.motor_a, 70);
        motor_channel_reverse(&drv.motor_b, 70);
        break;
    case 3:
        printf("\r\n>> brake\r\n");
        motor_channel_brake(&drv.motor_a);
        motor_channel_brake(&drv.motor_b);
        break;
    default:
        printf("\r\n>> coast\r\n");
        motor_channel_coast(&drv.motor_a);
        motor_channel_coast(&drv.motor_b);
        break;
    }
}


/**
 * @brief   Application entry point
 *
 * Initialises UART, QEI encoders, motor driver, and SysTick.  Prints
 * encoder position and velocity every PRINT_TICKS ticks.
 */
int main(void)
{
    uart0_init(115200);
    printf("\r\nmotor demo (sysclk %lu Hz)\r\n", (unsigned long)SystemCoreClock);


    qei_config_t enc = {
        .max_pos = 0xFFFFFFFFU,
        .vel_load = SystemCoreClock / VEL_WINDOW_HZ - 1U,
        .ctl_flags = QEI_CTL_CAPMODE
            | QEI_CTL_RESMODE
            | QEI_CTL_VELEN | QEI_CTL_VELDIV_1 | QEI_CTL_FILTEN,
    };

    qei_init(QEI0, enc);
    qei_init(QEI1, enc);
    qei_enable(QEI0);
    qei_enable(QEI1);

    motor_driver_init(&drv);
    motor_driver_wakeup(&drv);

    SysTick_Config(SystemCoreClock / TICK_HZ);
    demo_step(0);
    uint32_t last_tick = 0;
    while (1) {
        while (ticks == last_tick) {
            __WFI();
        }
        last_tick = ticks;
        if (ticks % PRINT_TICKS == 0) {
            uint32_t pos_a = qei_get_position(QEI0);
            uint32_t pos_b = qei_get_position(QEI1);
            uint32_t vel_a = qei_get_velocity(QEI0);
            uint32_t vel_b = qei_get_velocity(QEI1);
            printf("pos A=%lu, B=%lu, vel A=%lu, B=%lu\r\n",
                (unsigned long)pos_a, (unsigned long)pos_b,
                (unsigned long)vel_a, (unsigned long)vel_b);
        }
    }
}
