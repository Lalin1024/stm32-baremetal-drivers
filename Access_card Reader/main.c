#include "timer.h"
#include "uart.h"
#include "interrupt.h"

/* ================= GLOBAL VARIABLES ================= */

// Flag to indicate card data reception complete
volatile uint8_t read_flag = 0, em_flag = 0;

// Temporary variable (currently unused)
char a;

// Index for buffer storage (used inside ISR)
volatile int i = 0;

// Stores final decimal value of RFID card
volatile uint32_t deci = 0;

// Buffer to store raw UART data from EM-18 (expected ~12 bytes)
char buff[13];

// Stores extracted 8-character HEX card ID
char card[8];


/* ================= FUNCTION PROTOTYPE ================= */
void USART3_IRQHandler();

uint32_t hex_to_decimal(const char *hex)
{
    uint32_t result = 0;

    while(*hex)
    {
        char c = *hex++;

        // If character is '0' to '9'
        if(c >= '0' && c <= '9')
            result = (result << 4) | (c - '0');

        // If character is 'A' to 'F'
        else if(c >= 'A' && c <= 'F')
            result = (result << 4) | (c - 'A' + 10);

        // If character is 'a' to 'f'
        else if(c >= 'a' && c <= 'f')
            result = (result << 4) | (c - 'a' + 10);
    }

    return result;
}


int main()
{
    // Set system clock to 16 MHz
    SetClock_16MHz();

    // Initialize UART peripheral
    uart_init();

    // Enable USART3 interrupt for RX
    usart3_interrupt();

    while(1)
    {
        // Check if complete RFID frame is received
        if(em_flag)
        {
            em_flag = 0;   // Reset flag

            for(int j = 0; j < 8; j++)
            {
                card[j] = buff[j + 2];
            }

            // Convert HEX card ID to decimal
            deci = hex_to_decimal(card);

            // Send result over UART
            uart_write("Card: ");
            uart_write_int(deci);
            uart_write("\r\n");
        }
    }
}


/* ======================================================
   INTERRUPT HANDLER: USART3_IRQHandler

   PURPOSE:
   - Triggered when UART receives a byte
   - Stores incoming data into buffer
   - Detects end of RFID frame
   - Signals main loop via flag

   NOTE:
   RXNE (bit 5) indicates data available in RDR
====================================================== */
void USART3_IRQHandler()
{
    // Check if RXNE flag is set (data received)
    if(USART3->ISR & (1U << 5))
    {
        // Read received byte (clears RXNE automatically)
        char temp = USART3->RDR;

        // Store received byte into buffer
        buff[i++] = temp;

 
        if((buff[i] == '\0') && (i >= 12))
        {
            em_flag = 1;  // Signal data ready
            i = 0;        // Reset buffer index
        }
    }
}
