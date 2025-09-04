#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico_i2c_eeprom_lib.h"

// Default I2C address of 24LC32
#define I2C_ADDRESS 0x50

int main() {
    stdio_init_all();
    
    eeprom_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, 400*1000, i2c_default);

    // as a test, lets write a single byte then fill the page and read the result
    byte_write_eeprom(0x00, 19, I2C_ADDRESS);

    uint8_t data_list[31];
    for (int i = 0; i < 31; i++) {
        data_list[i] = 32;
    }

    page_write_eeprom(0x01, data_list, 31, I2C_ADDRESS);

    uint8_t result[32];
    read_eeprom(0x0, result, 32, I2C_ADDRESS);

    for (int i = 0; i < 32; i++) {
        printf("result: %i\n", result[i]);
    }
}