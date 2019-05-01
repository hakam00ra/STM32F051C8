#ifndef i2c
#define i2c

#include "stm32f051x8.h"

void i2c_transmit_cont (I2C_TypeDef *ic, uint8_t hbytes, uint8_t lbytes, uint8_t device, uint8_t address, uint64_t hdata, uint64_t ldata);

void i2c_transmit (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address, uint8_t data);

void i2c_init (I2C_TypeDef *ic);

uint64_t i2c_receive (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address);

uint64_t i2c_receive_cont (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address, uint64_t *phdata, uint64_t *pldata);


#endif

