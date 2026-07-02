/**
 * @file    qei.c
 * @brief   Quadrature Encoder Interface initialisation and control
 */

#include "motors/qei.h"

#define GPIO_LOCK_KEY 0x4C4F434BU

void qei_init(QEI0_Type *qei, qei_config_t cfg)
{
    if (qei == QEI0) {
        SYSCTL->RCGCQEI |= SYSCTL_RCGCQEI_R0;
        SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_QEI0_PORTD;
        while (!(SYSCTL->PRGPIO & SYSCTL_RCGCGPIO_QEI0_PORTD)) { }

        GPIOD->LOCK = GPIO_LOCK_KEY;
        GPIOD->CR |= QEI0_PHB_PIN_PD;
        GPIOD->LOCK = 0;

        GPIOD->AFSEL |= QEI0_PHA_PIN_PD | QEI0_PHB_PIN_PD;
        GPIOD->PCTL = (GPIOD->PCTL & ~((0xFU << 24) | (0xFU << 28)))
            | (QEI0_PCTL_AF << 24) | (QEI0_PCTL_AF << 28);
        GPIOD->DEN |= QEI0_PHA_PIN_PD | QEI0_PHB_PIN_PD;

        while (!(SYSCTL->PRQEI & SYSCTL_RCGCQEI_R0)) { }
    } else {
        SYSCTL->RCGCQEI |= SYSCTL_RCGCQEI_R1;
        SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_QEI1_PORTC;
        while (!(SYSCTL->PRGPIO & SYSCTL_RCGCGPIO_QEI1_PORTC)) { }

        GPIOC->AFSEL |= QEI1_PHA_PIN | QEI1_PHB_PIN;
        GPIOC->PCTL = (GPIOC->PCTL & ~((0xFU << 20) | (0xFU << 24)))
            | (QEI1_PCTL_AF << 20) | (QEI1_PCTL_AF << 24);
        GPIOC->DEN |= QEI1_PHA_PIN | QEI1_PHB_PIN;

        while (!(SYSCTL->PRQEI & SYSCTL_RCGCQEI_R1)) { }
    }

    qei->CTL = cfg.ctl_flags & ~QEI_CTL_ENABLE;
    qei->MAXPOS = cfg.max_pos;
    qei->LOAD = cfg.vel_load;
    qei->POS = 0;
}

void qei_enable(QEI0_Type *qei)
{
    qei->CTL |= QEI_CTL_ENABLE;
}

void qei_disable(QEI0_Type *qei)
{
    qei->CTL &= ~QEI_CTL_ENABLE;
}

uint32_t qei_get_position(QEI0_Type *qei)
{
    return qei->POS;
}

void qei_set_max_position(QEI0_Type *qei, uint32_t max_pos)
{
    qei->MAXPOS = max_pos;
}

int8_t qei_get_direction(QEI0_Type *qei)
{
    return (qei->STAT & QEI_STAT_DIRECTION) ? -1 : 1;
}

uint32_t qei_get_velocity(QEI0_Type *qei)
{
    return qei->SPEED;
}

void qei_enable_interrupt(QEI0_Type *qei, uint32_t int_mask)
{
    qei->INTEN |= int_mask;
}

void qei_disable_interrupt(QEI0_Type *qei, uint32_t int_mask)
{
    qei->INTEN &= ~int_mask;
}

void qei_clear_interrupt(QEI0_Type *qei, uint32_t int_mask)
{
    qei->ISC = int_mask;
}
