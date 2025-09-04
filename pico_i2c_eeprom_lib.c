#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Acknowledge polling can be used to see when a write cycle is complete
// Master sends start condition followed by control byte for write command
// i2c_write_blocking returning 1 indicated an acknowledgemenet, now we can continue (-1 for no ack)
// This function returns true when ack found, and false if timeout
int ack_poll(uint8_t i2c_addr) {
    int timeout = 100;
    for (int i = 0; i < timeout; i++) {
        if (i2c_write_blocking(i2c_default, i2c_addr, NULL, 1, false) == 1) {
            return true;
        }
        sleep_ms(1);
    }
    printf("ack timeout\n");
    return false;
}

void read_eeprom(uint16_t address, uint8_t *result, uint16_t num, uint8_t i2c_addr) {
    for (int i = 0; i < num; i++) {
        uint8_t byte1 = (uint8_t)(address >> 8);
        uint8_t byte2 = (uint8_t)(address);

        uint8_t buf[2] = {byte1, byte2};
        uint8_t res_buf[1];

        i2c_write_blocking(i2c_default, i2c_addr, buf, 2, true);
        i2c_read_blocking(i2c_default, i2c_addr, res_buf, 1, false);

        result[i] = res_buf[0];
        address++;
    }
}

// Write a single byte of data at a specified address in memory
void byte_write_eeprom(uint16_t address, uint8_t data, uint8_t i2c_addr) {
    uint8_t byte1 = (uint8_t)(address >> 8);
    uint8_t byte2 = (uint8_t)(address);

    uint8_t buf[3] = {byte1, byte2, data};

    i2c_write_blocking(i2c_default, i2c_addr, buf, 3, false);

    // ack_poll so when function exits can do stuff right away
    ack_poll(i2c_addr);
}

// Write a block of data to eeprom 
// Returns false if invalid data size passed in
int page_write_eeprom(uint16_t address, uint8_t *data, uint8_t num, uint8_t i2c_addr) {
    if (num > 32) {
        printf("Tried to write more than 4 pages of 8 bytes.\n");
        return false;
    }

    uint8_t byte1 = (uint8_t)(address >> 8);
    uint8_t byte2 = (uint8_t)(address);
    uint8_t buf[2 + num];
    buf[0] = byte1;
    buf[1] = byte2;

    // transfer data into buffer
    for (int i = 0; i < num; i++) {
        buf[i + 2] = data[i];
    }

    i2c_write_blocking(i2c_default, i2c_addr, buf, 2 + num, false);

    // ack_poll so when function exits can do stuff right away
    ack_poll(i2c_addr);
    return true;
}

// Initialise i2c for given gpio at given frequency
int eeprom_init(int sda_pin, int scl_pin, int freq, i2c_inst_t *i2c_instance) {
    i2c_init(i2c_default, 400*1000);
    
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

