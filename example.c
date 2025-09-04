#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "24lc32_i2c_lib.h"

// Default I2C address of 24LC32
#define I2C_ADDRESS 0x50

int main() {
    stdio_init_all();

    eeprom_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, 400*1000, i2c_default);

    // Write the byte at location 0x00 to 0
    byte_write_eeprom(0x00, 0, I2C_ADDRESS);

    // Make an array of integers from 1 to 31 and write the array to eeprom starting at 0x01
    uint8_t data_list[31];
    for (int i = 0; i < 31; i++) {
        data_list[i] = i + 1;
    }
    page_write_eeprom(0x01, data_list, 31, I2C_ADDRESS);

    // Read eeprom, and ensure we observe ascending integers from 0 to 31
    uint8_t result[32];
    read_eeprom(0x0, result, 32, I2C_ADDRESS);

    for (int i = 0; i < 32; i++) {
        printf("read: %i, expected: %i\n", result[i]), i;
        if (result[i] != i) {
            printf("Unexpected read!\n");
            return 1;
        }
    }
    
    printf("Test passed!\n");
    return 0;
}