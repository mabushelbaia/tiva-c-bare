/**
 * @file    uart.h
 * @brief   UART0 register bit definitions and API
 */

#ifndef UART_H
#define UART_H

#include "TM4C123GH6PM.h"
#include <stdint.h>

/* -----------------------------------------------------------------------
 * SYSCTL - Clock Gating
 * ----------------------------------------------------------------------- */
#define SYSCTL_RCGCUART_R0       (1U << 0)

/* -----------------------------------------------------------------------
 * UART Flag  (UARTx->FR)
 * ----------------------------------------------------------------------- */
#define UART_FR_CTS              (1U << 0)
#define UART_FR_BUSY             (1U << 3)
#define UART_FR_RXFE             (1U << 4)
#define UART_FR_TXFF             (1U << 5)
#define UART_FR_RXFF             (1U << 6)
#define UART_FR_TXFE             (1U << 7)

/* -----------------------------------------------------------------------
 * UART Line Control  (UARTx->LCRH)
 * ----------------------------------------------------------------------- */
#define UART_LCRH_BRK            (1U << 0)
#define UART_LCRH_PEN            (1U << 1)
#define UART_LCRH_EPS            (1U << 2)
#define UART_LCRH_STP2           (1U << 3)
#define UART_LCRH_FEN            (1U << 4)
#define UART_LCRH_WLEN_M         (3U << 5)
#define UART_LCRH_WLEN_5         (0U << 5)
#define UART_LCRH_WLEN_6         (1U << 5)
#define UART_LCRH_WLEN_7         (2U << 5)
#define UART_LCRH_WLEN_8         (3U << 5)
#define UART_LCRH_SPS            (1U << 7)

/* -----------------------------------------------------------------------
 * UART Control  (UARTx->CTL)
 * ----------------------------------------------------------------------- */
#define UART_CTL_UARTEN          (1U << 0)
#define UART_CTL_LBE             (1U << 7)
#define UART_CTL_TXE             (1U << 8)
#define UART_CTL_RXE             (1U << 9)

/* -----------------------------------------------------------------------
 * UART Interrupt bits
 * ----------------------------------------------------------------------- */
#define UART_INT_RX              (1U << 4)
#define UART_INT_TX              (1U << 5)
#define UART_INT_RT              (1U << 6)
#define UART_INT_FE              (1U << 7)
#define UART_INT_PE              (1U << 8)
#define UART_INT_BE              (1U << 9)
#define UART_INT_OE              (1U << 10)

/* -----------------------------------------------------------------------
 * UART Interrupt FIFO Level Select  (UARTx->IFLS)
 * ----------------------------------------------------------------------- */
#define UART_IFLS_TX_M           (7U << 0)
#define UART_IFLS_TX1_8          (0U << 0)
#define UART_IFLS_TX1_4          (1U << 0)
#define UART_IFLS_TX1_2          (2U << 0)
#define UART_IFLS_TX3_4          (3U << 0)
#define UART_IFLS_TX7_8          (4U << 0)
#define UART_IFLS_RX_M           (7U << 3)
#define UART_IFLS_RX1_8          (0U << 3)
#define UART_IFLS_RX1_4          (1U << 3)
#define UART_IFLS_RX1_2          (2U << 3)
#define UART_IFLS_RX3_4          (3U << 3)
#define UART_IFLS_RX7_8          (4U << 3)

/* -----------------------------------------------------------------------
 * UART Clock Configuration  (UARTx->CC)
 * ----------------------------------------------------------------------- */
#define UART_CC_SYSCLK          (0U << 0)
#define UART_CC_PIOSC            (5U << 0)

/* -----------------------------------------------------------------------
 * API
 * ----------------------------------------------------------------------- */

/**
 * @brief   Initialise UART0 on PA0 (U0RX) / PA1 (U0TX)
 *
 * Interrupt-driven TX with a non-blocking ring buffer.  uart0_putc
 * primes the hardware FIFO directly when the queue is empty, otherwise
 * enqueues; the ISR drains the queue as the FIFO empties.  Drops the
 * byte if the queue is full (see uart0_dropped).
 *
 * @param baud  Target baud rate
 */
void uart0_init(uint32_t baud);

/**
 * @brief   Transmit one character (non-blocking)
 *
 * @param c  Character to send
 */
void uart0_putc(char c);

/**
 * @brief   Transmit a null-terminated string
 *
 * @param s  String to send
 */
void uart0_puts(const char *s);

/**
 * @brief   Number of bytes discarded because the TX queue was full
 *
 * @return  Drop count
 */
uint32_t uart0_dropped(void);

#endif /* UART_H */
