/**
 * @file    qei.h
 * @brief   Quadrature Encoder Interface register bit definitions and API
 *
 * Supports QEI0 (PD6/PD7 default, PC4/PC6/PC7 alternate) and QEI1
 * (PC5/PC6) with configurable edge capture, velocity measurement, and
 * filtering.
 */

#ifndef QEI_H
#define QEI_H

#include "TM4C123GH6PM.h"
#include <stdint.h>

/* -----------------------------------------------------------------------
 * SYSCTL - Clock Gating
 * ----------------------------------------------------------------------- */
#define SYSCTL_RCGCQEI_R0        (1U << 0)
#define SYSCTL_RCGCQEI_R1        (1U << 1)

/* -----------------------------------------------------------------------
 * QEI Control  (QEIx->CTL)
 * ----------------------------------------------------------------------- */
#define QEI_CTL_ENABLE           (1U << 0)
#define QEI_CTL_SWAP             (1U << 1)
#define QEI_CTL_SIGMODE          (1U << 2)
#define QEI_CTL_CAPMODE          (1U << 3)
#define QEI_CTL_RESMODE          (1U << 4)
#define QEI_CTL_VELEN            (1U << 5)

#define QEI_CTL_VELDIV_M         (7U << 6)
#define QEI_CTL_VELDIV_1         (0U << 6)
#define QEI_CTL_VELDIV_2         (1U << 6)
#define QEI_CTL_VELDIV_4         (2U << 6)
#define QEI_CTL_VELDIV_8         (3U << 6)
#define QEI_CTL_VELDIV_16        (4U << 6)
#define QEI_CTL_VELDIV_32        (5U << 6)
#define QEI_CTL_VELDIV_64        (6U << 6)
#define QEI_CTL_VELDIV_128       (7U << 6)

#define QEI_CTL_INVA             (1U << 9)
#define QEI_CTL_INVB             (1U << 10)
#define QEI_CTL_INVI             (1U << 11)
#define QEI_CTL_STALLEN          (1U << 12)
#define QEI_CTL_FILTEN           (1U << 13)

#define QEI_CTL_FILTCNT_M        (0xFU << 16)
#define QEI_CTL_FILTCNT_S        16

/* -----------------------------------------------------------------------
 * QEI Status  (QEIx->STAT)
 * ----------------------------------------------------------------------- */
#define QEI_STAT_ERROR           (1U << 0)
#define QEI_STAT_DIRECTION       (1U << 1)

/* -----------------------------------------------------------------------
 * QEI Interrupt  (QEIx->INTEN, RIS, ISC)
 * ----------------------------------------------------------------------- */
#define QEI_INT_INDEX            (1U << 0)
#define QEI_INT_TIMER            (1U << 1)
#define QEI_INT_DIR              (1U << 2)
#define QEI_INT_ERROR            (1U << 3)

/* -----------------------------------------------------------------------
 * GPIO Alternate Function — QEI0 (PCTL = 6)
 *
 * Default: PhA0 = PD6, PhB0 = PD7, IDX0 = PD3
 * Alt:     PhA0 = PC4, PhB0 = PC6, IDX0 = PC7 / PF4
 * ----------------------------------------------------------------------- */
#define QEI0_PCTL_AF             6U

#define QEI0_PHA_PIN_PD          (1U << 6)
#define QEI0_PHB_PIN_PD          (1U << 7)
#define QEI0_IDX_PIN_PD          (1U << 3)

#define QEI0_PHA_PIN_PC          (1U << 4)
#define QEI0_PHB_PIN_PC          (1U << 6)
#define QEI0_IDX_PIN_PC          (1U << 7)

/* GPIO Alternate Function — QEI1 (PCTL = 6)
 * PhA1 = PC5, PhB1 = PC6, IDX1 = PC4
 * ----------------------------------------------------------------------- */
#define QEI1_PCTL_AF             6U

#define QEI1_PHA_PIN             (1U << 5)
#define QEI1_PHB_PIN             (1U << 6)
#define QEI1_IDX_PIN             (1U << 4)

/* -----------------------------------------------------------------------
 * SYSCTL RCGC bits for GPIO ports used by QEI
 * ----------------------------------------------------------------------- */
#define SYSCTL_RCGCGPIO_QEI0_PORTD   (1U << 3)
#define SYSCTL_RCGCGPIO_QEI0_PORTC   (1U << 2)
#define SYSCTL_RCGCGPIO_QEI1_PORTC   (1U << 2)

/* -----------------------------------------------------------------------
 * Types and API
 * ----------------------------------------------------------------------- */

/**
 * @brief  QEI module configuration
 * @param  max_pos    MAXPOS — position wraps/resets at this value
 * @param  vel_load   Velocity timer load (pulse count period)
 * @param  ctl_flags  OR of QEI_CTL_* flags
 */
typedef struct {
    uint32_t max_pos;
    uint32_t vel_load;
    uint32_t ctl_flags;
} qei_config_t;

/**
 * @brief  Initialise a QEI module
 *
 * Enables clocks, configures GPIO alternate function, and writes the
 * control, MAXPOS, and velocity LOAD registers.  Does NOT enable the
 * QEI — call qei_enable() when ready.
 *
 * @param qei  QEI module (QEI0 or QEI1)
 * @param cfg  Configuration
 */
void qei_init(QEI0_Type *qei, qei_config_t cfg);

/**
 * @brief  Enable the QEI (starts position tracking)
 *
 * @param qei  QEI module
 */
void qei_enable(QEI0_Type *qei);

/**
 * @brief  Disable the QEI
 *
 * @param qei  QEI module
 */
void qei_disable(QEI0_Type *qei);

/**
 * @brief  Read the current position counter
 *
 * @param  qei  QEI module
 * @return      Position value from the POS register
 */
uint32_t qei_get_position(QEI0_Type *qei);

/**
 * @brief  Update the maximum position value
 *
 * @param qei      QEI module
 * @param max_pos  New MAXPOS value
 */
void qei_set_max_position(QEI0_Type *qei, uint32_t max_pos);

/**
 * @brief  Read the rotation direction
 *
 * @param  qei  QEI module
 * @return      +1 = forward (counting up), -1 = reverse (counting down)
 */
int8_t qei_get_direction(QEI0_Type *qei);

/**
 * @brief  Read the velocity (raw pulse count)
 *
 * Returns the SPEED register value: number of pulses counted over the
 * last velocity timer period.  Requires VELEN in ctl_flags at init.
 *
 * @param  qei  QEI module
 * @return      Raw velocity count
 */
uint32_t qei_get_velocity(QEI0_Type *qei);

/**
 * @brief  Enable QEI interrupts
 *
 * @param qei       QEI module
 * @param int_mask  OR of QEI_INT_* bits
 */
void qei_enable_interrupt(QEI0_Type *qei, uint32_t int_mask);

/**
 * @brief  Disable QEI interrupts
 *
 * @param qei       QEI module
 * @param int_mask  OR of QEI_INT_* bits
 */
void qei_disable_interrupt(QEI0_Type *qei, uint32_t int_mask);

/**
 * @brief  Clear QEI interrupt flags
 *
 * @param qei       QEI module
 * @param int_mask  OR of QEI_INT_* bits
 */
void qei_clear_interrupt(QEI0_Type *qei, uint32_t int_mask);

#endif /* QEI_H */
