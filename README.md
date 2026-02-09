# STM32 Bare-Metal Drivers (STM32L433RCT6P)

This repository contains bare-metal peripheral drivers and example application
code developed for the **STM32L433RCT6P** microcontroller.

The project is implemented using direct register-level programming.
No STM32 HAL or LL drivers are used.

Only CMSIS core and device header files are included to access processor
and peripheral register definitions.

---

## Target Microcontroller

- Microcontroller: STM32L433RCT6P
- Core: ARM Cortex-M4 with FPU
- System Clock: 16 MHz (HSI)
- Vendor: STMicroelectronics

---

## Development Approach

- Bare-metal embedded C programming
- Direct access to peripheral registers
- No abstraction layers such as HAL or LL
- CMSIS used only for:
  - Cortex-M core definitions
  - Peripheral register structures
  - Interrupt vector and NVIC definitions

STM32CubeIDE is used only as a build and debug environment.
No HAL or LL drivers are used in the application code.

---

## CMSIS Usage

This project uses CMSIS (Cortex Microcontroller Software Interface Standard)
for STM32L4 devices.

CMSIS provides:
- ARM Cortex-M core support
- Device-specific register definitions

All peripheral configuration is performed manually using registers.

CMSIS files can be obtained from:
- STMicroelectronics official website (https://www.st.com)
- STM32Cube firmware packages for the STM32L4 series

---

## Project Structure

stm32-baremetal-drivers/
├── adc/ # ADC bare-metal driver
├── uart/ # UART bare-metal driver
├── i2c/ # I2C bare-metal driver
├── timer/ # Timer and delay driver
├── interrupt/ # EXTI and timer interrupt handling
├── CMSIS/ # CMSIS core and device headers
├── main.c # Application entry point
└── README.md
