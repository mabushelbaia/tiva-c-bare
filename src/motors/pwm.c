/**
 * @file    pwm.c
 * @brief   PWM module initialisation and control
 */

#include "motors/pwm.h"

#define GEN_CTL  0
#define GEN_LOAD 4
#define GEN_CMPA 6
#define GEN_CMPB 7
#define GEN_GENA 8
#define GEN_GENB 9

/**
 * @brief   Return a pointer to the generator's register block
 *
 * @param  pwm  PWM module
 * @param  gen  Generator index (0-3)
 * @return      Pointer to the _n_CTL register
 */
static volatile uint32_t *gen_block(PWM0_Type *pwm, uint8_t gen)
{
    return &pwm->_0_CTL + (uint32_t)gen * 16U;
}

void pwm_init(PWM0_Type *pwm)
{
    uint32_t module = (pwm == PWM0) ? SYSCTL_RCGCPWM_R0 : SYSCTL_RCGCPWM_R1;

    SYSCTL->RCGCPWM |= module;
    while (!(SYSCTL->PRPWM & module)) { }

    SYSCTL->RCC &= ~SYSCTL_RCC_USEPWMDIV;

    if (pwm == PWM0) {
        SYSCTL->RCGCGPIO |= (1U << 1);
        while (!(SYSCTL->PRGPIO & (1U << 1))) { }

        GPIOB->AFSEL |= PWM0_GEN0A_PIN | PWM0_GEN0B_PIN;
        GPIOB->PCTL = (GPIOB->PCTL & ~((0xFU << 24) | (0xFU << 28)))
            | (PWM0_PCTL_AF << 24) | (PWM0_PCTL_AF << 28);
        GPIOB->DEN |= PWM0_GEN0A_PIN | PWM0_GEN0B_PIN;
    }
}

void pwm_gen_init(PWM0_Type *pwm, uint8_t gen, pwm_gen_config_t cfg)
{
    volatile uint32_t *g = gen_block(pwm, gen);

    g[GEN_CTL] = cfg.mode & PWM_GEN_CTL_MODE;

    g[GEN_GENA] = PWM_GEN_GEN_ACTLOAD_HIGH | PWM_GEN_GEN_ACTCMPAD_LOW;
    g[GEN_GENB] = PWM_GEN_GEN_ACTLOAD_HIGH | PWM_GEN_GEN_ACTCMPBD_LOW;

    g[GEN_LOAD] = cfg.period;
    g[GEN_CMPA] = cfg.period;
    g[GEN_CMPB] = cfg.period;
}

void pwm_set_period(PWM0_Type *pwm, uint8_t gen, uint32_t period)
{
    gen_block(pwm, gen)[GEN_LOAD] = period;
}

void pwm_set_duty_a(PWM0_Type *pwm, uint8_t gen, uint32_t cmpa)
{
    gen_block(pwm, gen)[GEN_CMPA] = cmpa;
}

void pwm_set_duty_b(PWM0_Type *pwm, uint8_t gen, uint32_t cmpb)
{
    gen_block(pwm, gen)[GEN_CMPB] = cmpb;
}

void pwm_gen_enable(PWM0_Type *pwm, uint8_t gen)
{
    gen_block(pwm, gen)[GEN_CTL] |= PWM_GEN_CTL_ENABLE;
}

void pwm_gen_disable(PWM0_Type *pwm, uint8_t gen)
{
    gen_block(pwm, gen)[GEN_CTL] &= ~PWM_GEN_CTL_ENABLE;
}

void pwm_output_enable(PWM0_Type *pwm, uint8_t output_mask)
{
    pwm->ENABLE |= output_mask;
}

void pwm_output_disable(PWM0_Type *pwm, uint8_t output_mask)
{
    pwm->ENABLE &= ~output_mask;
}
