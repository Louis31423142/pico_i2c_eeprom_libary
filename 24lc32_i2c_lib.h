#ifndef PICO_I2C_EEPROM_LIB_H
#define PICO_I2C_EEPROM_LIB_H

#include "hardware/i2c.h"

void read_eeprom(uint16_t address, uint8_t *result, uint16_t num, uint8_t i2c_addr);
void byte_write_eeprom(uint16_t address, uint8_t data, uint8_t i2c_addr);
bool page_write_eeprom(uint16_t address, uint8_t *data, uint8_t num, uint8_t i2c_addr);
bool ack_poll(uint8_t i2c_addr);
void eeprom_init(int sda_pin, int scl_pin, int freq, i2c_inst_t *i2c_instance);

#endif