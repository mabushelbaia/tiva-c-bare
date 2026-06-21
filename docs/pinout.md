# EK-TM4C123GXL pinout

On-board peripherals used (or handy to know) on the Launchpad:

| Pin | Function   | Notes                                            |
|-----|------------|--------------------------------------------------|
| PF1 | GPIO out   | on-board RGB LED, red, active high               |
| PF2 | GPIO out   | on-board RGB LED, blue, active high              |
| PF3 | GPIO out   | on-board RGB LED, green, active high (blinky)    |
| PF4 | GPIO in    | on-board switch SW1, active low (needs pull-up)  |
| PF0 | GPIO in    | on-board switch SW2, active low; NMI pin (locked at reset) |
| PA0 | U0RX       | ICDI virtual COM port (115200 8N1)               |
| PA1 | U0TX       | ICDI virtual COM port (115200 8N1)               |

The UART shows up on the host as `/dev/ttyACM0` (Linux) over the on-board ICDI
USB connection.
