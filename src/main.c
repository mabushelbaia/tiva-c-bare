#include "TM4C123.h"

#define GREEN_LED (1 << 3) // PF3 corresponds to the green LED
#define BLUE_LED (1 << 2) // PF2 corresponds to the blue LED
#define RED_LED (1 << 1) // PF1 corresponds to the red LED
#define RCGCGPIO_R5 (1 << 5) // Bit 5 corresponds to GPIOF
#define RCGCTIMER_R1 (1 << 1) // Bit 1 corresponds to Timer1

uint8_t colors[] = {
    GREEN_LED, // Green LED
    BLUE_LED, // Blue LED
    RED_LED, // Red LED
    RED_LED | GREEN_LED, // Yellow LED (Red + Green)
    RED_LED | BLUE_LED, // Magenta LED (Red + Blue)
    GREEN_LED | BLUE_LED, // Cyan LED (Green + Blue)
    RED_LED | GREEN_LED | BLUE_LED // White LED (Red + Green + Blue)
};
uint8_t color_index = 0; // Index to track the current color

int main(void)
{
    SYSCTL->RCGCGPIO |= RCGCGPIO_R5; // Enable clock to GPIOF
    SYSCTL->RCGCTIMER |= RCGCTIMER_R1; // Enable clock to Timer1

    __asm__("NOP"); // Wait for clock stabilization
    __asm__("NOP"); // Wait for clock stabilization
    __asm__("NOP"); // Wait for clock stabilization

    GPIOF->DIR |= BLUE_LED | GREEN_LED | RED_LED; // Set blue LED as output pin
    GPIOF->DEN |= BLUE_LED | GREEN_LED | RED_LED; // Enable digital function for all LEDs

    TIMER1->CTL = 0; // Disable the timer
    TIMER1->CFG = 0x4; // Choose 16-bit mode
    TIMER1->TAMR = 0x02; // Periodic mode
    TIMER1->TAPR = 256 - 1; // Set prescaler value
    TIMER1->TAILR = 65536 - 1; // Set initial reload value
    TIMER1->ICR = 0x1; // Clear any prior interrupts
    TIMER1->IMR |= (1 << 0); // Enable timeout interrupt

    NVIC_EnableIRQ(TIMER1A_IRQn); // Enable Timer1A interrupt in NVIC
    TIMER1->CTL |= 0x01; // Enable the timer

    while (1) // Main loop
    { }
}

void TIMER1A_Handler()
{
    if (TIMER1->MIS & 0x1) { // Check if timer timeout interrupt occurred
        GPIOF->DATA = colors[color_index & 0x7]; // Cycle through colors
        color_index = (color_index + 1) % 7;
        TIMER1->ICR = 0x1; // Clear timer interrupt flag
    }
}