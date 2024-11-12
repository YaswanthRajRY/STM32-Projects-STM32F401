# BSP Development for STM32F401xC (UART, SPI, I2C in Baremetal)

## Description

This repository contains the Board Support Package (BSP) developed for the **STM32F401xC** microcontroller, designed to provide low-level hardware interface support for **UART**, **SPI**, and **I2C** peripherals in a **baremetal** environment. The project includes custom drivers and configurations for these protocols, without the use of higher-level libraries, ensuring optimized performance and reduced resource overhead.

## Features
- **UART Driver**: Implemented for serial communication.
- **SPI Driver**: Enabled for high-speed peripheral communication.
- **I2C Driver**: Configured for communication with I2C-based sensors and devices.
- **Interrupt Service Routines (ISRs)**: Efficient ISRs for real-time data transmission and reception.
- **Makefile**: Includes a Makefile to automate the build and compilation process for the project.

## Technologies Used
- **Microcontroller**: STM32F401xC
- **Programming Language**: C
- **Development Environment**: Baremetal (No external libraries)
- **Debugging Tools**: STM-Link, GDB
- **Build Automation**: Makefile
