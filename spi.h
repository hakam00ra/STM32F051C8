#ifndef spi
#define spi

#include "stm32f051x8.h"


// configuration
#define master ((uint32_t) 0x00)
#define slave ((uint32_t) 0x01)

// polarity and phase
#define pol0 ((uint32_t) 0x00)
#define pol1 ((uint32_t) 0x01)
#define ph0 ((uint32_t) 0x00)
#define ph1 ((uint32_t) 0x01)

// data size (not all listed)
#define bits8 ((uint32_t) 0x07)
#define bits16 ((uint32_t) 0x0F)

// FIFO threshold
#define th16 ((uint32_t) 0x00)
#define th8 ((uint32_t) 0x01)


typedef struct
{
SPI_TypeDef *sp;
uint32_t mas;
uint32_t pol;
uint32_t ph;
uint32_t bit;
uint32_t fifo;
}SPI_TYPE;




void spi_init  (SPI_TYPE spi_type);

void spi_transmit(SPI_TypeDef*sp, uint32_t bytes, uint32_t data);

uint64_t spi_receive(SPI_TypeDef*sp, uint32_t bytes);






























#endif

