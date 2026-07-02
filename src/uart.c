/**
 * @file    uart.c
 * @brief   UART0 interrupt-driven TX driver
 *
 * Implements a non-blocking transmit path backed by a ring buffer.
 * The _putchar() backend lets printf() from mpaland/printf route through
 * the same TX queue.
 */

#include "uart.h"
#include "printf.h"

#define TX_BUF_SIZE 256U
#define TX_BUF_MASK (TX_BUF_SIZE - 1U)

static uint8_t tx_buf[TX_BUF_SIZE];
static volatile uint32_t tx_head;
static volatile uint32_t tx_tail;
static volatile uint32_t tx_drops;

/**
 * @brief   Initialise UART0 at the given baud rate
 *
 * Enables clocks, configures PA0 (U0RX) / PA1 (U0TX) as alternate
 * function, sets the baud rate, enables 8N1 with FIFOs, and enables
 * the TX interrupt.
 *
 * @param baud  Target baud rate (e.g. 115200)
 */
void uart0_init(uint32_t baud)
{
    SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R0;
    SYSCTL->RCGCGPIO |= (1U << 0);
    while (!(SYSCTL->PRGPIO & (1U << 0))) { }

    GPIOA->AFSEL |= (1U << 0) | (1U << 1);
    GPIOA->PCTL = (GPIOA->PCTL & ~0xFFU) | 0x11U;
    GPIOA->DEN |= (1U << 0) | (1U << 1);

    while (!(SYSCTL->PRUART & SYSCTL_RCGCUART_R0)) { }

    UART0->CTL &= ~UART_CTL_UARTEN;

    uint32_t brd64 = (SystemCoreClock * 4U + baud / 2U) / baud;
    UART0->IBRD = brd64 >> 6;
    UART0->FBRD = brd64 & 0x3FU;

    UART0->LCRH = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART0->IFLS = (UART0->IFLS & ~UART_IFLS_TX_M) | UART_IFLS_TX1_2;
    UART0->IM = UART_INT_TX;

    NVIC_EnableIRQ(UART0_IRQn);

    UART0->CTL = UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE;
}

/**
 * @brief   Send one character (non-blocking)
 *
 * Writes directly to the hardware FIFO when the queue is empty and the
 * FIFO has room, otherwise enqueues the byte.  Drops the byte if the
 * ring buffer is full.
 *
 * @param c  Character to transmit
 */
void uart0_putc(char c)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    if (tx_head == tx_tail && !(UART0->FR & UART_FR_TXFF)) {
        UART0->DR = (uint8_t)c;
    } else {
        uint32_t next = (tx_head + 1U) & TX_BUF_MASK;
        if (next != tx_tail) {
            tx_buf[tx_head] = (uint8_t)c;
            tx_head = next;
        } else {
            tx_drops++;
        }
    }

    if (!primask) {
        __enable_irq();
    }
}

/**
 * @brief   Send a null-terminated string
 *
 * @param s  String to transmit
 */
void uart0_puts(const char *s)
{
    while (*s) {
        uart0_putc(*s++);
    }
}

/**
 * @brief   Get the number of bytes dropped due to a full TX queue
 *
 * @return  Drop count
 */
uint32_t uart0_dropped(void)
{
    return tx_drops;
}

/**
 * @brief   printf backend — called by mpaland/printf
 *
 * @param character  Character to output
 */
void _putchar(char character)
{
    uart0_putc(character);
}

/**
 * @brief   UART0 interrupt handler
 *
 * Drains the TX ring buffer into the hardware FIFO.
 */
void UART0_Handler(void)
{
    UART0->ICR = UART_INT_TX;

    while (tx_tail != tx_head && !(UART0->FR & UART_FR_TXFF)) {
        UART0->DR = tx_buf[tx_tail];
        tx_tail = (tx_tail + 1U) & TX_BUF_MASK;
    }
}
