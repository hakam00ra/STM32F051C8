#ifndef uart
#define uart

#include "stm32f051x8.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"








void uart_init(uint8_t uar);
void print(uint8_t uar, char*msg, ...);





#endif




