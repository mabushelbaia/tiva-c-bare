#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "TM4C123GH6PM.h"
#include "motors/pwm.h"
#include <stdint.h>

/**
 * @file    motor_driver.h
 * @brief   TB6612FNG dual DC motor driver control
 *
 * Truth table (per motor channel):
 *   IN1  IN2  PWM   Mode
 *   H    L    PWM   Forward (CW)
 *   L    H    PWM   Reverse (CCW)
 *   H    H    H/L   Short brake
 *   L    L    H/L   Coast (free-wheel)
 *
 * The STBY pin is active-low: pull low to put the whole IC into standby.
 */

/**
 * @brief Per-motor-channel handle
 *
 * Holds the two direction GPIO pins and the PWM output that drives one
 * motor channel of the TB6612FNG.
 *
 * @param in1_port  GPIO port for IN1
 * @param in1_pin   Pin mask for IN1
 * @param in2_port  GPIO port for IN2
 * @param in2_pin   Pin mask for IN2
 * @param pwm       PWM module (PWM0 or PWM1)
 * @param gen       PWM generator index (0-3)
 * @param pwm_ch    0 = generator channel A (CMPA), 1 = channel B (CMPB)
 * @param period    LOAD value the generator was initialised with
 */
typedef struct {
    GPIOA_Type *in1_port;
    uint8_t     in1_pin;

    GPIOA_Type *in2_port;
    uint8_t     in2_pin;

    PWM0_Type  *pwm;
    uint8_t     gen;
    uint8_t     pwm_ch;
    uint32_t    period;
} motor_channel_t;

/**
 * @brief Dual motor driver handle
 *
 * Holds the STBY GPIO and two motor channels.  Both motors share one
 * STBY pin.
 *
 * @param stby_port  GPIO port for STBY
 * @param stby_pin   Pin mask for STBY
 * @param motor_a    First motor channel
 * @param motor_b    Second motor channel
 */
typedef struct {
    GPIOA_Type    *stby_port;
    uint8_t        stby_pin;

    motor_channel_t motor_a;
    motor_channel_t motor_b;
} motor_driver_t;

/* -----------------------------------------------------------------------
 * Driver-level init / standby
 * ----------------------------------------------------------------------- */

/**
 * @brief Initialise the motor driver
 *
 * Configures the STBY pin as a digital output, initialises the PWM
 * module(s), and sets up both motor channels.  Leaves the IC in standby
 * (STBY low).
 *
 * @param drv  Driver handle
 */
void motor_driver_init(motor_driver_t *drv);

/**
 * @brief Enter standby mode
 *
 * Drives STBY low; both motors coast and the IC draws minimal current.
 *
 * @param drv  Driver handle
 */
void motor_driver_standby(motor_driver_t *drv);

/**
 * @brief Exit standby mode
 *
 * Drives STBY high; the IC is active and motors can be driven.
 *
 * @param drv  Driver handle
 */
void motor_driver_wakeup(motor_driver_t *drv);

/* -----------------------------------------------------------------------
 * Per-motor-channel init
 * ----------------------------------------------------------------------- */

/**
 * @brief Initialise a single motor channel
 *
 * Configures IN1/IN2 as digital outputs, initialises the PWM generator,
 * and enables the PWM output pin.  Does not call motor_driver_wakeup.
 *
 * @param m  Motor channel handle
 */
void motor_channel_init(motor_channel_t *m);

/* -----------------------------------------------------------------------
 * Per-motor-channel control
 * ----------------------------------------------------------------------- */

/**
 * @brief Drive the motor forward
 *
 * IN1=H, IN2=L.  Duty 0 = stopped, 100 = full speed.
 *
 * @param m         Motor channel handle
 * @param duty_pct  Duty cycle percentage (0-100)
 */
void motor_channel_forward(motor_channel_t *m, uint8_t duty_pct);

/**
 * @brief Drive the motor in reverse
 *
 * IN1=L, IN2=H.  Duty 0 = stopped, 100 = full speed.
 *
 * @param m         Motor channel handle
 * @param duty_pct  Duty cycle percentage (0-100)
 */
void motor_channel_reverse(motor_channel_t *m, uint8_t duty_pct);

/**
 * @brief Short brake
 *
 * IN1=H, IN2=H — motor terminals are shorted together.
 *
 * @param m  Motor channel handle
 */
void motor_channel_brake(motor_channel_t *m);

/**
 * @brief Coast (free-wheel)
 *
 * IN1=L, IN2=L.  PWM is forced to 100% to prevent the TB6612 from
 * short-braking when the PWM input goes low.
 *
 * @param m  Motor channel handle
 */
void motor_channel_coast(motor_channel_t *m);

/**
 * @brief Set motor speed without changing direction
 *
 * Writes a new duty cycle to the PWM generator without touching the
 * IN1/IN2 pins.
 *
 * @param m         Motor channel handle
 * @param duty_pct  Duty cycle percentage (0-100)
 */
void motor_channel_set_speed(motor_channel_t *m, uint8_t duty_pct);

#endif /* MOTOR_DRIVER_H */
