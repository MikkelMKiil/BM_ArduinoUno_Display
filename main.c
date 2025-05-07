#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


/*
 * ATmega328P Microcontroller Bare-Metal Example
 * Description: This program demonstrates basic serial (USART) communication setup
 *              and message transmission. It also disables the watchdog timer,
 *              which can be helpful during development to prevent unintended resets.
 * Target:      ATmega328P
 * Compiler:    AVR-GCC
*/

/*
    Function: initSerial
    Description: Initializes the USART0 peripheral for serial communication.
    Input:
        ubrr_value: The pre-calculated UBRR value for the desired baud rate.
                    This value is directly loaded into UBRR0L.
        speed_mode: Configures USART speed.
                    0 for normal speed (U2X0 = 0).
                    1 for double speed (U2X0 = 1), often preferred for accuracy.
    Output: None. USART0 registers are configured.
*/
void initSerial(int baudrate, int speed) {
    // Set USART Double Speed Operation (U2X0 bit in UCSR0A) based on 'speed_mode'.
    UCSR0A = (speed << U2X0);
    /*
     Set the UBRR0L register with the lower byte of the UBRR value.
     UBRR0H is implicitly set to 0, assuming UBRR value fits in UBRR0L (<= 255).
     Refer to the ATmega328P datasheet for UBRR calculation tables/formulas.
    */
    UBRR0L = baudrate;
    UBRR0H = 0;
    // Enable Transmitter (TXEN0) and Receiver (RXEN0) in UCSR0B.
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // Set frame format in UCSR0C: 8 data bits, 1 stop bit (default), no parity.
    // UCSZ01 and UCSZ00 = 1 for 8-bit character size.
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void serialWrite(unsigned char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}
void serialWriteString(const char * str) {
    while ( * str) {
        serialWrite( * str++);
    }
}

/*
 * Watchdog Timer (WDT) Disable:
 * The WDT is disabled to prevent the microcontroller from resetting
 * unexpectedly during program execution or debugging. On some Arduino
 * platforms, an active WDT without periodic resets can cause the
 * device to enter a reset loop (often indicated by rapid LED flashing).
*/
// RS  =  Chip select aka    CS
// R/W =  Read/Write  aka    SID
// E   =  E           aka    SCLK
int main() {
    wdt_disable();
        // Optional: Disable global interrupts (sei() to enable).
        // Useful if precise timing is needed and no interrupt-driven
        // peripherals are active initially.
    // cli(); This turns off interrupts if you need it.
    initSerial(16,1);
    serialWriteString("\r\nATMEGA328P Ready !!!\r\n");
    return 0;           // Program termination (in a bare-metal context, main usually loops).
}