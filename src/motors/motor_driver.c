#include "motors/motor_driver.h"

/**
 * @brief Enable the clock for a GPIO port
 *
 * @param port  GPIO port pointer (GPIOA through GPIOF)
 */
static void gpio_clock_enable(GPIOA_Type *port)
{
    uint32_t bit;

    if (port == GPIOA) {
        bit = 1U << 0;
    } else if (port == GPIOB) {
        bit = 1U << 1;
    } else if (port == GPIOC) {
        bit = 1U << 2;
    } else if (port == GPIOD) {
        bit = 1U << 3;
    } else if (port == GPIOE) {
        bit = 1U << 4;
    } else {
        bit = 1U << 5; /* GPIOF */
    }

    SYSCTL->RCGCGPIO |= bit;
    while (!(SYSCTL->PRGPIO & bit)) { }
}

/**
 * @brief Configure a GPIO pin as a digital output, initially low
 *
 * @param port  GPIO port
 * @param pin   Pin mask
 */
static void gpio_output_init(GPIOA_Type *port, uint8_t pin)
{
    gpio_clock_enable(port);
    port->AFSEL &= ~(uint32_t)pin;
    port->DIR |= pin;
    port->DEN |= pin;
    port->DATA &= ~(uint32_t)pin;
}

/**
 * @brief Convert a duty percentage to a raw PWM compare value
 *
 * Count-down mode with ACTLOAD=HIGH / ACTCMPxD=LOW means:
 * compare == LOAD gives 0% (compare action outranks load action) and
 * compare == 0 gives ~100% (one PWM clock short of full).
 *
 * @param m         Motor channel (provides period)
 * @param duty_pct  Duty 0-100
 * @return  Raw compare value for CMPA/CMPB
 */
static uint32_t duty_to_cmp(const motor_channel_t *m, uint8_t duty_pct)
{
    if (duty_pct >= 100U) {
        return 0U;
    }
    return m->period * (100U - duty_pct) / 100U;
}

/**
 * @brief Write a duty cycle to the PWM generator
 *
 * Routes to CMPA or CMPB depending on m->pwm_ch.
 *
 * @param m         Motor channel handle
 * @param duty_pct  Duty 0-100
 */
static void set_duty(motor_channel_t *m, uint8_t duty_pct)
{
    uint32_t cmp = duty_to_cmp(m, duty_pct);

    if (m->pwm_ch == 0U) {
        pwm_set_duty_a(m->pwm, m->gen, cmp);
    } else {
        pwm_set_duty_b(m->pwm, m->gen, cmp);
    }
}

void motor_driver_init(motor_driver_t *drv)
{
    gpio_output_init(drv->stby_port, drv->stby_pin);

    pwm_init(drv->motor_a.pwm);
    if (drv->motor_b.pwm != drv->motor_a.pwm) {
        pwm_init(drv->motor_b.pwm);
    }

    motor_channel_init(&drv->motor_a);
    motor_channel_init(&drv->motor_b);
}

void motor_driver_standby(motor_driver_t *drv)
{
    drv->stby_port->DATA &= ~(uint32_t)drv->stby_pin;
}

void motor_driver_wakeup(motor_driver_t *drv)
{
    drv->stby_port->DATA |= drv->stby_pin;
}

void motor_channel_init(motor_channel_t *m)
{
    pwm_gen_config_t cfg = { .period = m->period, .mode = 0 };

    gpio_output_init(m->in1_port, m->in1_pin);
    gpio_output_init(m->in2_port, m->in2_pin);

    pwm_gen_init(m->pwm, m->gen, cfg);
    pwm_gen_enable(m->pwm, m->gen);
    pwm_output_enable(m->pwm, (uint8_t)(1U << (m->gen * 2U + m->pwm_ch)));
}

void motor_channel_forward(motor_channel_t *m, uint8_t duty_pct)
{
    m->in1_port->DATA |= m->in1_pin;
    m->in2_port->DATA &= ~(uint32_t)m->in2_pin;
    set_duty(m, duty_pct);
}

void motor_channel_reverse(motor_channel_t *m, uint8_t duty_pct)
{
    m->in1_port->DATA &= ~(uint32_t)m->in1_pin;
    m->in2_port->DATA |= m->in2_pin;
    set_duty(m, duty_pct);
}

void motor_channel_brake(motor_channel_t *m)
{
    m->in1_port->DATA |= m->in1_pin;
    m->in2_port->DATA |= m->in2_pin;
}

void motor_channel_coast(motor_channel_t *m)
{
    m->in1_port->DATA &= ~(uint32_t)m->in1_pin;
    m->in2_port->DATA &= ~(uint32_t)m->in2_pin;
    set_duty(m, 100U);
}

void motor_channel_set_speed(motor_channel_t *m, uint8_t duty_pct)
{
    set_duty(m, duty_pct);
}
