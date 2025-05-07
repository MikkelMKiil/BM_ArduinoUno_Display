#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "st7920.h"

void initSerial(int baudrate, int speed) {
    UCSR0A = (speed << U2X0);
    UBRR0L = baudrate;
    UBRR0H = 0;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void serialWrite(unsigned char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void serialWriteString(const char *str) {
    while (*str) {
        serialWrite(*str++);
    }
}

int main() {
    wdt_disable();
    initSerial(16, 1);
    init_st7920();
    serialWriteString("\r\nATMEGA328P Ready !!!\r\n");

    // Example: Write "Hello" to the display
    st7920_send_command(0x80); // Set DDRAM address to 0x00 (start of first line)
    st7920_send_data('2');
    st7920_send_data('1');
    st7920_send_data('1');
    st7920_send_data('3');
    while (1) {
        // Main loop
    }
    return 0;
}