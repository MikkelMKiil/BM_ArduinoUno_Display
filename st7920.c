#include "st7920.h"

void init_control_pins() {
    // Set pins 13 (PB5), 11 (PB3), and 10 (PB2) as output
    DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2);
}

void set_control_pin_level(uint8_t pin_b_num, uint8_t level) {
    if (level) {
        PORTB |= (1 << pin_b_num); // Set pin high
    } else {
        PORTB &= ~(1 << pin_b_num); // Set pin low
    }
}

static void st7920_serial_send_raw_byte(uint8_t byte_val) {
    for (uint8_t i = 0; i < 8; i++) {
        // Set SID pin (data bit)
        if (byte_val & 0x80) { // Check MSB
            set_control_pin_level(ST7920_SID_PIN, 1);
        } else {
            set_control_pin_level(ST7920_SID_PIN, 0);
        }
        byte_val <<= 1; // Shift to next bit
        set_control_pin_level(ST7920_SCLK_PIN, 0); // Ensure SCLK is low
        _delay_us(1); // Small delay for setup time, adjust if necessary
        set_control_pin_level(ST7920_SCLK_PIN, 1); // SCLK high: ST7920 latches data
        _delay_us(1); // Small delay for SCLK high pulse width
        set_control_pin_level(ST7920_SCLK_PIN, 0); // SCLK low
        _delay_us(1); // Small delay before next bit or end of byte
    }
}

void st7920_serial_write(uint8_t value, uint8_t is_data) {
    uint8_t control_packet = 0b11111000; // Sync (11111), RW=0 (Write)
    if (is_data) {
        control_packet |= 0b00000010; // Set RS bit (RS=1 for data)
    }
    st7920_serial_send_raw_byte(control_packet);
    uint8_t high_nibble_packet = (value & 0xF0); // Higher 4 bits of value, lower 4 bits are already 0
    st7920_serial_send_raw_byte(high_nibble_packet);
    uint8_t low_nibble_packet = ((value << 4) & 0xF0); // Lower 4 bits of value shifted to high nibble, new lower 4 bits are 0
    st7920_serial_send_raw_byte(low_nibble_packet);
    _delay_us(80); // Minimum delay, adjust based on ST7920 datasheet and F_CPU
}

void st7920_send_command(uint8_t cmd) {
    st7920_serial_write(cmd, 0); // is_data = 0 for command
}

void st7920_send_data(uint8_t data) {
    st7920_serial_write(data, 1); // is_data = 1 for data
}

void init_st7920() {
    init_control_pins(); // Initialize control pins (PB5, PB3, PB2 as output)
    set_control_pin_level(ST7920_CS_PIN, 1); // Set CS (PB2) high
    _delay_ms(40); // Wait for LCD to power up
    st7920_send_command(0x30); // Function Set: 8-bit interface (repeated for basic mode)
    _delay_us(100);
    st7920_send_command(0x30); // Function Set: 8-bit interface
    _delay_us(40);
    st7920_send_command(0x0C); // Display ON, Cursor OFF, Blink OFF
    _delay_us(100);
    st7920_send_command(0x01); // Clear Display
    _delay_ms(2);
    st7920_send_command(0x06); // Entry Mode Set: Increment cursor, no shift
    _delay_us(100);
}