#ifndef ST7920_H
#define ST7920_H

#include <avr/io.h>
#include <util/delay.h>

// Pin Definitions
#define ST7920_CS_PIN   PB2  // Chip Select (Arduino Pin 10)
#define ST7920_SID_PIN  PB3  // Serial Data In / MOSI (Arduino Pin 11)
#define ST7920_SCLK_PIN PB5  // Serial Clock (Arduino Pin 13)

// Function Prototypes
void init_control_pins(void);
void set_control_pin_level(uint8_t pin_b_num, uint8_t level);
void st7920_serial_write(uint8_t value, uint8_t is_data);
void st7920_send_command(uint8_t cmd);
void st7920_send_data(uint8_t data);
void init_st7920(void);

#endif // ST7920_H