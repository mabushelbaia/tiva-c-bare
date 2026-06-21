/**************************************************************************//**
 * @file     startup_TM4C123.c
 * @brief    CMSIS-style startup for the TM4C123GH6PM: vector table + reset
 *           handler. Each device/exception vector is a weak alias to
 *           Default_Handler, so an application can override any of them just by
 *           defining a function of the same name (e.g. TIMER1A_Handler in
 *           src/main.c). The slot names match the IRQn_Type enum in
 *           device/TM4C123GH6PM.h.
 *
 *           Reset_Handler copies .data from flash to SRAM, zeroes .bss, runs
 *           SystemInit() (clock + FPU setup), then calls main(). The linker
 *           script (linker.ld) provides the segment bounds and _estack.
 *****************************************************************************/

#include <stdint.h>

/* Provided by linker.ld */
extern uint32_t _ldata; /* load address of .data in flash */
extern uint32_t _data;  /* .data start in SRAM */
extern uint32_t _edata; /* .data end in SRAM */
extern uint32_t _bss;   /* .bss start in SRAM */
extern uint32_t _ebss;  /* .bss end in SRAM */
extern uint32_t _estack; /* top of stack (end of SRAM) */

/* Provided by device/system_TM4C123.c and the application */
extern void SystemInit(void);
extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/* Cortex-M4 system exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* TM4C123GH6PM device interrupts (see IRQn_Type in TM4C123GH6PM.h) */
void GPIOA_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOB_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOD_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOE_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSI0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_FAULT_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void QEI0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC0SS0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC0SS1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC0SS2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC0SS3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WATCHDOG0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER0A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER0B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER1A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER1B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER2A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER2B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void COMP1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SYSCTL_Handler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_CTRL_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOF_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER3A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER3B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void QEI1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HIB_Handler(void) __attribute__((weak, alias("Default_Handler")));
void USB0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM0_3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UDMA_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UDMAERR_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1SS0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1SS1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1SS2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1SS3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSI2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SSI3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART7_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER4A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER4B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER5A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER5B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER0A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER0B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER1A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER1B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER2A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER2B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER3A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER3B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER4A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER4B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER5A_Handler(void) __attribute__((weak, alias("Default_Handler")));
void WTIMER5B_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SYSEXC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PWM1_FAULT_Handler(void) __attribute__((weak, alias("Default_Handler")));

typedef void (*pFunc)(void);

/* The vector table. KEEP()'d and placed at 0x00000000 by linker.ld. The first
 * entry is the initial stack pointer; the CPU loads it on reset. */
__attribute__((section(".isr_vector"), used))
const pFunc g_pfnVectors[] = {
    (pFunc)(&_estack),           /*     Initial stack pointer            */
    Reset_Handler,               /*     Reset                            */
    NMI_Handler,                 /* -14 Non-maskable interrupt           */
    HardFault_Handler,           /* -13 Hard fault                       */
    MemManage_Handler,           /* -12 Memory management                */
    BusFault_Handler,            /* -11 Bus fault                        */
    UsageFault_Handler,          /* -10 Usage fault                      */
    0,                           /*     Reserved                         */
    0,                           /*     Reserved                         */
    0,                           /*     Reserved                         */
    0,                           /*     Reserved                         */
    SVC_Handler,                 /*  -5 SVCall                           */
    DebugMon_Handler,            /*  -4 Debug monitor                    */
    0,                           /*     Reserved                         */
    PendSV_Handler,              /*  -2 PendSV                           */
    SysTick_Handler,             /*  -1 SysTick                          */
    GPIOA_Handler,               /*   0 GPIO Port A                      */
    GPIOB_Handler,               /*   1 GPIO Port B                      */
    GPIOC_Handler,               /*   2 GPIO Port C                      */
    GPIOD_Handler,               /*   3 GPIO Port D                      */
    GPIOE_Handler,               /*   4 GPIO Port E                      */
    UART0_Handler,               /*   5 UART0                            */
    UART1_Handler,               /*   6 UART1                            */
    SSI0_Handler,                /*   7 SSI0                             */
    I2C0_Handler,                /*   8 I2C0                             */
    PWM0_FAULT_Handler,          /*   9 PWM0 Fault                       */
    PWM0_0_Handler,              /*  10 PWM0 Generator 0                 */
    PWM0_1_Handler,              /*  11 PWM0 Generator 1                 */
    PWM0_2_Handler,              /*  12 PWM0 Generator 2                 */
    QEI0_Handler,                /*  13 Quadrature Encoder 0             */
    ADC0SS0_Handler,             /*  14 ADC0 Sequence 0                  */
    ADC0SS1_Handler,             /*  15 ADC0 Sequence 1                  */
    ADC0SS2_Handler,             /*  16 ADC0 Sequence 2                  */
    ADC0SS3_Handler,             /*  17 ADC0 Sequence 3                  */
    WATCHDOG0_Handler,           /*  18 Watchdog Timer 0                 */
    TIMER0A_Handler,             /*  19 Timer 0 subtimer A               */
    TIMER0B_Handler,             /*  20 Timer 0 subtimer B               */
    TIMER1A_Handler,             /*  21 Timer 1 subtimer A               */
    TIMER1B_Handler,             /*  22 Timer 1 subtimer B               */
    TIMER2A_Handler,             /*  23 Timer 2 subtimer A               */
    TIMER2B_Handler,             /*  24 Timer 2 subtimer B               */
    COMP0_Handler,               /*  25 Analog Comparator 0              */
    COMP1_Handler,               /*  26 Analog Comparator 1              */
    0,                           /*  27 Reserved                         */
    SYSCTL_Handler,              /*  28 System Control                   */
    FLASH_CTRL_Handler,          /*  29 Flash + EEPROM Control           */
    GPIOF_Handler,               /*  30 GPIO Port F                      */
    0,                           /*  31 Reserved                         */
    0,                           /*  32 Reserved                         */
    UART2_Handler,               /*  33 UART2                            */
    SSI1_Handler,                /*  34 SSI1                             */
    TIMER3A_Handler,             /*  35 Timer 3 subtimer A               */
    TIMER3B_Handler,             /*  36 Timer 3 subtimer B               */
    I2C1_Handler,                /*  37 I2C1                             */
    QEI1_Handler,                /*  38 Quadrature Encoder 1             */
    CAN0_Handler,                /*  39 CAN0                             */
    CAN1_Handler,                /*  40 CAN1                             */
    0,                           /*  41 Reserved                         */
    0,                           /*  42 Reserved                         */
    HIB_Handler,                 /*  43 Hibernation Module               */
    USB0_Handler,                /*  44 USB0                             */
    PWM0_3_Handler,              /*  45 PWM0 Generator 3                 */
    UDMA_Handler,                /*  46 uDMA Software Transfer           */
    UDMAERR_Handler,             /*  47 uDMA Error                       */
    ADC1SS0_Handler,             /*  48 ADC1 Sequence 0                  */
    ADC1SS1_Handler,             /*  49 ADC1 Sequence 1                  */
    ADC1SS2_Handler,             /*  50 ADC1 Sequence 2                  */
    ADC1SS3_Handler,             /*  51 ADC1 Sequence 3                  */
    0,                           /*  52 Reserved                         */
    0,                           /*  53 Reserved                         */
    0,                           /*  54 Reserved                         */
    0,                           /*  55 Reserved                         */
    0,                           /*  56 Reserved                         */
    SSI2_Handler,                /*  57 SSI2                             */
    SSI3_Handler,                /*  58 SSI3                             */
    UART3_Handler,               /*  59 UART3                            */
    UART4_Handler,               /*  60 UART4                            */
    UART5_Handler,               /*  61 UART5                            */
    UART6_Handler,               /*  62 UART6                            */
    UART7_Handler,               /*  63 UART7                            */
    0,                           /*  64 Reserved                         */
    0,                           /*  65 Reserved                         */
    0,                           /*  66 Reserved                         */
    0,                           /*  67 Reserved                         */
    I2C2_Handler,                /*  68 I2C2                             */
    I2C3_Handler,                /*  69 I2C3                             */
    TIMER4A_Handler,             /*  70 Timer 4 subtimer A               */
    TIMER4B_Handler,             /*  71 Timer 4 subtimer B               */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 72-81  Reserved                     */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 82-91  Reserved                     */
    TIMER5A_Handler,             /*  92 Timer 5 subtimer A               */
    TIMER5B_Handler,             /*  93 Timer 5 subtimer B               */
    WTIMER0A_Handler,            /*  94 Wide Timer 0 subtimer A          */
    WTIMER0B_Handler,            /*  95 Wide Timer 0 subtimer B          */
    WTIMER1A_Handler,            /*  96 Wide Timer 1 subtimer A          */
    WTIMER1B_Handler,            /*  97 Wide Timer 1 subtimer B          */
    WTIMER2A_Handler,            /*  98 Wide Timer 2 subtimer A          */
    WTIMER2B_Handler,            /*  99 Wide Timer 2 subtimer B          */
    WTIMER3A_Handler,            /* 100 Wide Timer 3 subtimer A          */
    WTIMER3B_Handler,            /* 101 Wide Timer 3 subtimer B          */
    WTIMER4A_Handler,            /* 102 Wide Timer 4 subtimer A          */
    WTIMER4B_Handler,            /* 103 Wide Timer 4 subtimer B          */
    WTIMER5A_Handler,            /* 104 Wide Timer 5 subtimer A          */
    WTIMER5B_Handler,            /* 105 Wide Timer 5 subtimer B          */
    SYSEXC_Handler,              /* 106 System Exception (imprecise)     */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 107-116 Reserved                    */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 117-126 Reserved                    */
    0, 0, 0, 0, 0, 0, 0,          /* 127-133 Reserved                    */
    PWM1_0_Handler,              /* 134 PWM1 Generator 0                 */
    PWM1_1_Handler,              /* 135 PWM1 Generator 1                 */
    PWM1_2_Handler,              /* 136 PWM1 Generator 2                 */
    PWM1_3_Handler,              /* 137 PWM1 Generator 3                 */
    PWM1_FAULT_Handler,          /* 138 PWM1 Fault                       */
};

/* Entry point on reset. Sets up the C runtime, then jumps to main(). */
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy initialised data from its load address in flash to SRAM. */
    for (src = &_ldata, dst = &_data; dst < &_edata;)
    {
        *dst++ = *src++;
    }

    /* Zero-fill the .bss segment. */
    for (dst = &_bss; dst < &_ebss;)
    {
        *dst++ = 0;
    }

    /* Clock + FPU configuration before any code that may use the FPU. */
    SystemInit();

    main();

    /* main() should never return; trap if it does. */
    while (1)
    {
    }
}

/* Default handler for any unoverridden interrupt: spin so a debugger can
 * inspect the state. */
void Default_Handler(void)
{
    while (1)
    {
    }
}
