# EM-18 RFID Card Reader Driver (125KHz)

This project contains firmware drivers for reading **125KHz RFID access cards** using the **EM-18 RFID Reader Module**.

## 📌 Overview
The system reads RFID card IDs via UART communication from the EM-18 module, processes the received hexadecimal data, and converts it into a decimal format for easy identification and usage.

## ⚙️ Features
- Reads RFID card data from EM-18 (125KHz)
- UART interrupt-based data reception
- Hexadecimal to decimal conversion of card ID
- Simple and efficient parsing of card data
- Real-time processing using embedded interrupts

## 🧩 Dependencies
- UART driver for serial communication
- Timer driver (used from the `timers/` folder for system timing and control)

## 📁 Project Structure
- `uart.c / uart.h` → UART communication handling  
- `interrupt.c` → Interrupt configuration and ISR handling  
- `timer/` → Timer driver module (used for delays/timing control)  
- `main.c` → Core logic for RFID card processing  

## 🚀 Working Principle
1. EM-18 module reads 125KHz RFID card
2. Data is sent via UART to the microcontroller
3. UART interrupt stores incoming bytes into a buffer
4. Once full card data is received, it is extracted and processed
5. Hex ID is converted into decimal and printed over UART

## 📡 Output
The system prints the RFID card number in decimal format via UART.

Example : 12345678
