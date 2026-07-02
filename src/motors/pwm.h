/**
 * @file    pwm.h
 * @brief   PWM module register bit definitions and API
 *
 * Supports both PWM0 and PWM1 modules with generators 0-3, count-down
 * and count-up/down modes, and all output action encodings.
 */

#ifndef PWM_H
#define PWM_H

#include "TM4C123GH6PM.h"
#include <stdint.h>

/* -----------------------------------------------------------------------
 * SYSCTL - Clock Gating
 * ----------------------------------------------------------------------- */
#define SYSCTL_RCGCPWM_R0        (1U << 0)
#define SYSCTL_RCGCPWM_R1        (1U << 1)

/* -----------------------------------------------------------------------
 * PWM Master Control  (PWMx->CTL)
 * ----------------------------------------------------------------------- */
#define PWM_CTL_GLOBALSYNC0      (1U << 0)
#define PWM_CTL_GLOBALSYNC1      (1U << 1)
#define PWM_CTL_GLOBALSYNC2      (1U << 2)
#define PWM_CTL_GLOBALSYNC3      (1U << 3)

/* -----------------------------------------------------------------------
 * PWM Time Base Sync  (PWMx->SYNC)
 * ----------------------------------------------------------------------- */
#define PWM_SYNC_SYNC0           (1U << 0)
#define PWM_SYNC_SYNC1           (1U << 1)
#define PWM_SYNC_SYNC2           (1U << 2)
#define PWM_SYNC_SYNC3           (1U << 3)

/* -----------------------------------------------------------------------
 * PWM Output Enable  (PWMx->ENABLE)
 * ----------------------------------------------------------------------- */
#define PWM_ENABLE_PWM0EN        (1U << 0)
#define PWM_ENABLE_PWM1EN        (1U << 1)
#define PWM_ENABLE_PWM2EN        (1U << 2)
#define PWM_ENABLE_PWM3EN        (1U << 3)
#define PWM_ENABLE_PWM4EN        (1U << 4)
#define PWM_ENABLE_PWM5EN        (1U << 5)
#define PWM_ENABLE_PWM6EN        (1U << 6)
#define PWM_ENABLE_PWM7EN        (1U << 7)

/* -----------------------------------------------------------------------
 * PWM Output Inversion  (PWMx->INVERT)
 * ----------------------------------------------------------------------- */
#define PWM_INVERT_PWM0INV       (1U << 0)
#define PWM_INVERT_PWM1INV       (1U << 1)
#define PWM_INVERT_PWM2INV       (1U << 2)
#define PWM_INVERT_PWM3INV       (1U << 3)
#define PWM_INVERT_PWM4INV       (1U << 4)
#define PWM_INVERT_PWM5INV       (1U << 5)
#define PWM_INVERT_PWM6INV       (1U << 6)
#define PWM_INVERT_PWM7INV       (1U << 7)

/* -----------------------------------------------------------------------
 * PWM Output Fault  (PWMx->FAULT)
 * ----------------------------------------------------------------------- */
#define PWM_FAULT_FAULT0         (1U << 0)
#define PWM_FAULT_FAULT1         (1U << 1)
#define PWM_FAULT_FAULT2         (1U << 2)
#define PWM_FAULT_FAULT3         (1U << 3)
#define PWM_FAULT_FAULT4         (1U << 4)
#define PWM_FAULT_FAULT5         (1U << 5)
#define PWM_FAULT_FAULT6         (1U << 6)
#define PWM_FAULT_FAULT7         (1U << 7)

/* -----------------------------------------------------------------------
 * PWM Interrupt / Status  (PWMx->INTEN, RIS, ISC)
 * ----------------------------------------------------------------------- */
#define PWM_INT_INTPWM0          (1U << 0)
#define PWM_INT_INTPWM1          (1U << 1)
#define PWM_INT_INTPWM2          (1U << 2)
#define PWM_INT_INTPWM3          (1U << 3)
#define PWM_INT_INTFAULT0        (1U << 16)
#define PWM_INT_INTFAULT1        (1U << 17)
#define PWM_INT_INTFAULT2        (1U << 18)
#define PWM_INT_INTFAULT3        (1U << 19)

/* -----------------------------------------------------------------------
 * PWM Status  (PWMx->STATUS)
 * ----------------------------------------------------------------------- */
#define PWM_STATUS_FAULT0        (1U << 0)
#define PWM_STATUS_FAULT1        (1U << 1)
#define PWM_STATUS_FAULT2        (1U << 2)
#define PWM_STATUS_FAULT3        (1U << 3)

/* -----------------------------------------------------------------------
 * PWM Fault Condition Value  (PWMx->FAULTVAL)
 * ----------------------------------------------------------------------- */
#define PWM_FAULTVAL_PWM0        (1U << 0)
#define PWM_FAULTVAL_PWM1        (1U << 1)
#define PWM_FAULTVAL_PWM2        (1U << 2)
#define PWM_FAULTVAL_PWM3        (1U << 3)
#define PWM_FAULTVAL_PWM4        (1U << 4)
#define PWM_FAULTVAL_PWM5        (1U << 5)
#define PWM_FAULTVAL_PWM6        (1U << 6)
#define PWM_FAULTVAL_PWM7        (1U << 7)

/* -----------------------------------------------------------------------
 * PWM Enable Update  (PWMx->ENUPD)
 * ----------------------------------------------------------------------- */
#define PWM_ENUPD_IMM            0x0U
#define PWM_ENUPD_LSYNC          0x2U
#define PWM_ENUPD_GSYNC          0x3U

#define PWM_ENUPD_ENUPD0_S       0
#define PWM_ENUPD_ENUPD1_S       2
#define PWM_ENUPD_ENUPD2_S       4
#define PWM_ENUPD_ENUPD3_S       6
#define PWM_ENUPD_ENUPD4_S       8
#define PWM_ENUPD_ENUPD5_S       10
#define PWM_ENUPD_ENUPD6_S       12
#define PWM_ENUPD_ENUPD7_S       14

/* -----------------------------------------------------------------------
 * Per-Generator Control  (PWMx->_n_CTL)
 * ----------------------------------------------------------------------- */
#define PWM_GEN_CTL_ENABLE       (1U << 0)
#define PWM_GEN_CTL_MODE         (1U << 1)
#define PWM_GEN_CTL_DEBUG        (1U << 2)
#define PWM_GEN_CTL_LOADUPD      (1U << 3)
#define PWM_GEN_CTL_CMPAUPD      (1U << 4)
#define PWM_GEN_CTL_CMPBUPD      (1U << 5)

#define PWM_GEN_CTL_GENAUPD_M    (3U << 6)
#define PWM_GEN_CTL_GENAUPD_IMM  (0U << 6)
#define PWM_GEN_CTL_GENAUPD_LSYNC (2U << 6)
#define PWM_GEN_CTL_GENAUPD_GSYNC (3U << 6)
#define PWM_GEN_CTL_GENBUPD_M    (3U << 8)
#define PWM_GEN_CTL_GENBUPD_IMM  (0U << 8)
#define PWM_GEN_CTL_GENBUPD_LSYNC (2U << 8)
#define PWM_GEN_CTL_GENBUPD_GSYNC (3U << 8)

#define PWM_GEN_CTL_DBCTLUPD_M   (3U << 10)
#define PWM_GEN_CTL_DBCTLUPD_IMM (0U << 10)
#define PWM_GEN_CTL_DBCTLUPD_LSYNC (2U << 10)
#define PWM_GEN_CTL_DBCTLUPD_GSYNC (3U << 10)
#define PWM_GEN_CTL_DBRISEUPD_M  (3U << 12)
#define PWM_GEN_CTL_DBRISEUPD_IMM (0U << 12)
#define PWM_GEN_CTL_DBRISEUPD_LSYNC (2U << 12)
#define PWM_GEN_CTL_DBRISEUPD_GSYNC (3U << 12)
#define PWM_GEN_CTL_DBFALLUPD_M  (3U << 14)
#define PWM_GEN_CTL_DBFALLUPD_IMM (0U << 14)
#define PWM_GEN_CTL_DBFALLUPD_LSYNC (2U << 14)
#define PWM_GEN_CTL_DBFALLUPD_GSYNC (3U << 14)

#define PWM_GEN_CTL_FLTSRC       (1U << 16)
#define PWM_GEN_CTL_MINFLTPER    (1U << 17)
#define PWM_GEN_CTL_LATCH        (1U << 18)

/* -----------------------------------------------------------------------
 * Per-Generator Interrupt/Trigger  (PWMx->_n_INTEN, _n_RIS, _n_ISC)
 * ----------------------------------------------------------------------- */
#define PWM_GEN_INT_INTCNTZERO   (1U << 0)
#define PWM_GEN_INT_INTCNTLOAD   (1U << 1)
#define PWM_GEN_INT_INTCMPAU     (1U << 2)
#define PWM_GEN_INT_INTCMPAD     (1U << 3)
#define PWM_GEN_INT_INTCMPBU     (1U << 4)
#define PWM_GEN_INT_INTCMPBD     (1U << 5)
#define PWM_GEN_INT_TRCNTZERO    (1U << 8)
#define PWM_GEN_INT_TRCNTLOAD    (1U << 9)
#define PWM_GEN_INT_TRCMPAU      (1U << 10)
#define PWM_GEN_INT_TRCMPAD      (1U << 11)
#define PWM_GEN_INT_TRCMPBU      (1U << 12)
#define PWM_GEN_INT_TRCMPBD      (1U << 13)

/* -----------------------------------------------------------------------
 * Per-Generator Output Action  (PWMx->_n_GENA, PWMx->_n_GENB)
 *
 * 2-bit fields: 00=none, 01=invert, 10=drive-low, 11=drive-high
 * ----------------------------------------------------------------------- */
#define PWM_GEN_GEN_ACTZERO_S    0
#define PWM_GEN_GEN_ACTLOAD_S    2
#define PWM_GEN_GEN_ACTCMPAU_S   4
#define PWM_GEN_GEN_ACTCMPAD_S   6
#define PWM_GEN_GEN_ACTCMPBU_S   8
#define PWM_GEN_GEN_ACTCMPBD_S   10

#define PWM_GEN_ACT_NONE         0x0U
#define PWM_GEN_ACT_INV          0x1U
#define PWM_GEN_ACT_LOW          0x2U
#define PWM_GEN_ACT_HIGH         0x3U

#define PWM_GEN_GEN_ACTZERO_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTZERO_S)
#define PWM_GEN_GEN_ACTZERO_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTZERO_S)
#define PWM_GEN_GEN_ACTZERO_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTZERO_S)
#define PWM_GEN_GEN_ACTZERO_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTZERO_S)

#define PWM_GEN_GEN_ACTLOAD_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTLOAD_S)
#define PWM_GEN_GEN_ACTLOAD_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTLOAD_S)
#define PWM_GEN_GEN_ACTLOAD_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTLOAD_S)
#define PWM_GEN_GEN_ACTLOAD_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTLOAD_S)

#define PWM_GEN_GEN_ACTCMPAU_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTCMPAU_S)
#define PWM_GEN_GEN_ACTCMPAU_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTCMPAU_S)
#define PWM_GEN_GEN_ACTCMPAU_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTCMPAU_S)
#define PWM_GEN_GEN_ACTCMPAU_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTCMPAU_S)

#define PWM_GEN_GEN_ACTCMPAD_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTCMPAD_S)
#define PWM_GEN_GEN_ACTCMPAD_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTCMPAD_S)
#define PWM_GEN_GEN_ACTCMPAD_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTCMPAD_S)
#define PWM_GEN_GEN_ACTCMPAD_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTCMPAD_S)

#define PWM_GEN_GEN_ACTCMPBU_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTCMPBU_S)
#define PWM_GEN_GEN_ACTCMPBU_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTCMPBU_S)
#define PWM_GEN_GEN_ACTCMPBU_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTCMPBU_S)
#define PWM_GEN_GEN_ACTCMPBU_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTCMPBU_S)

#define PWM_GEN_GEN_ACTCMPBD_NONE (PWM_GEN_ACT_NONE << PWM_GEN_GEN_ACTCMPBD_S)
#define PWM_GEN_GEN_ACTCMPBD_INV  (PWM_GEN_ACT_INV  << PWM_GEN_GEN_ACTCMPBD_S)
#define PWM_GEN_GEN_ACTCMPBD_LOW  (PWM_GEN_ACT_LOW  << PWM_GEN_GEN_ACTCMPBD_S)
#define PWM_GEN_GEN_ACTCMPBD_HIGH (PWM_GEN_ACT_HIGH << PWM_GEN_GEN_ACTCMPBD_S)

/* -----------------------------------------------------------------------
 * Per-Generator Dead-Band  (PWMx->_n_DBCTL)
 * ----------------------------------------------------------------------- */
#define PWM_GEN_DBCTL_ENABLE     (1U << 0)

/* -----------------------------------------------------------------------
 * GPIO Alternate Function — PWM0 (PCTL = 4)
 *
 * Gen0: M0PWM0 = PB6, M0PWM1 = PB7
 * Gen1: M0PWM2 = PB4, M0PWM3 = PB5
 * Gen2: M0PWM4 = PE4, M0PWM5 = PE5
 * Gen3: M0PWM6 = PC4, M0PWM7 = PC5
 * Fault: M0FAULT0 = PD6 / PF4
 * ----------------------------------------------------------------------- */
#define PWM0_PCTL_AF             4U

#define PWM0_GEN0A_PIN           (1U << 6)
#define PWM0_GEN0B_PIN           (1U << 7)
#define PWM0_GEN1A_PIN           (1U << 4)
#define PWM0_GEN1B_PIN           (1U << 5)
#define PWM0_GEN2A_PIN           (1U << 4)
#define PWM0_GEN2B_PIN           (1U << 5)
#define PWM0_GEN3A_PIN           (1U << 4)
#define PWM0_GEN3B_PIN           (1U << 5)

/* GPIO Alternate Function — PWM1 (PCTL = 5)
 * Gen0: M1PWM0 = PD0, M1PWM1 = PD1
 * Gen1: M1PWM2 = PA6, M1PWM3 = PA7
 * Gen2: M1PWM4 = PF0, M1PWM5 = PF1
 * Gen3: M1PWM6 = PF2, M1PWM7 = PF3
 * Fault: M1FAULT0 = PF4
 * ----------------------------------------------------------------------- */
#define PWM1_PCTL_AF             5U

#define PWM1_GEN0A_PIN           (1U << 0)
#define PWM1_GEN0B_PIN           (1U << 1)
#define PWM1_GEN1A_PIN           (1U << 6)
#define PWM1_GEN1B_PIN           (1U << 7)
#define PWM1_GEN2A_PIN           (1U << 0)
#define PWM1_GEN2B_PIN           (1U << 1)
#define PWM1_GEN3A_PIN           (1U << 2)
#define PWM1_GEN3B_PIN           (1U << 3)

/* -----------------------------------------------------------------------
 * PWM Clock Divider  (SYSCTL->RCC)
 * ----------------------------------------------------------------------- */
#define SYSCTL_RCC_USEPWMDIV     (1U << 20)
#define SYSCTL_RCC_PWMDIV_M      (7U << 17)
#define SYSCTL_RCC_PWMDIV_2      (0U << 17)
#define SYSCTL_RCC_PWMDIV_4      (1U << 17)
#define SYSCTL_RCC_PWMDIV_8      (2U << 17)
#define SYSCTL_RCC_PWMDIV_16     (3U << 17)
#define SYSCTL_RCC_PWMDIV_32     (4U << 17)
#define SYSCTL_RCC_PWMDIV_64     (5U << 17)

/* -----------------------------------------------------------------------
 * Types and API
 * ----------------------------------------------------------------------- */

/**
 * @brief  PWM generator configuration
 * @param  period  LOAD value (period in counts - 1)
 * @param  mode    0 = count-down, PWM_GEN_CTL_MODE = count-up/down
 */
typedef struct {
    uint32_t period;
    uint8_t  mode;
} pwm_gen_config_t;

/**
 * @brief  Initialise a PWM module
 *
 * Enables the module clock, clears the PWM divider to run at full system
 * clock, and sets up the GPIO alternate function for PB6/PB7 (PWM0 Gen0).
 *
 * @param pwm  PWM module (PWM0 or PWM1)
 */
void pwm_init(PWM0_Type *pwm);

/**
 * @brief  Initialise a PWM generator
 *
 * Configures count mode, output actions (active-high PWM), period, and
 * initial 0% duty on both channels.
 *
 * @param pwm  PWM module
 * @param gen  Generator index (0-3)
 * @param cfg  Period and mode configuration
 */
void pwm_gen_init(PWM0_Type *pwm, uint8_t gen, pwm_gen_config_t cfg);

/**
 * @brief  Set the generator period
 *
 * @param pwm     PWM module
 * @param gen     Generator index
 * @param period  LOAD value
 */
void pwm_set_period(PWM0_Type *pwm, uint8_t gen, uint32_t period);

/**
 * @brief  Set channel A compare value (duty cycle)
 *
 * In count-down mode, duty = (LOAD - CMPA) / LOAD.
 *
 * @param pwm   PWM module
 * @param gen   Generator index
 * @param cmpa  Compare A value
 */
void pwm_set_duty_a(PWM0_Type *pwm, uint8_t gen, uint32_t cmpa);

/**
 * @brief  Set channel B compare value (duty cycle)
 *
 * @param pwm   PWM module
 * @param gen   Generator index
 * @param cmpb  Compare B value
 */
void pwm_set_duty_b(PWM0_Type *pwm, uint8_t gen, uint32_t cmpb);

/**
 * @brief  Enable a PWM generator (start its counter)
 *
 * @param pwm  PWM module
 * @param gen  Generator index
 */
void pwm_gen_enable(PWM0_Type *pwm, uint8_t gen);

/**
 * @brief  Disable a PWM generator (stop its counter)
 *
 * @param pwm  PWM module
 * @param gen  Generator index
 */
void pwm_gen_disable(PWM0_Type *pwm, uint8_t gen);

/**
 * @brief  Enable PWM output pins
 *
 * @param pwm          PWM module
 * @param output_mask  OR of PWM_ENABLE_PWMxEN bits
 */
void pwm_output_enable(PWM0_Type *pwm, uint8_t output_mask);

/**
 * @brief  Disable PWM output pins
 *
 * @param pwm          PWM module
 * @param output_mask  OR of PWM_ENABLE_PWMxEN bits
 */
void pwm_output_disable(PWM0_Type *pwm, uint8_t output_mask);

#endif /* PWM_H */
