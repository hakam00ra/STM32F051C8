#ifndef gpio
#define gpio

#include "stm32f051x8.h"

// MODER REGISTER
#define input_mode  ((uint32_t) 0x00)
#define output_mode ((uint32_t) 0x01)
#define alt_func    ((uint32_t) 0x02)
#define analog_mode ((uint32_t) 0x03)

// OTYPER REGISTER
#define pp ((uint32_t) 0x00)
#define od ((uint32_t) 0x01)

//PUPDR
#define nopupd ((uint32_t) 0x00)
#define pu ((uint32_t) 0x01)
#define pd ((uint32_t) 0x02)

//OSPEEDDR
#define ls ((uint32_t) 0x01)
#define ms ((uint32_t) 0x02)
#define hs ((uint32_t) 0x03)


typedef struct
{
GPIO_TypeDef *gp;
uint32_t pin;
uint32_t mode;
uint32_t type;
uint32_t speed;
uint32_t af;
uint32_t res;
}GPIO_TYPE;

typedef enum{
RISING_EDGE,
FALLING_EDGE,
RISING_FALLING_EDGE
}edge_select;

void gpio_init(GPIO_TYPE gpio_type);


void gpio_write(GPIO_TypeDef *gp, uint32_t pinNumber, uint8_t state);
void gpio_toggle(GPIO_TypeDef *gp, uint32_t pinNumber);

void configure_gpio_interrupt(GPIO_TypeDef*gp, uint32_t pinNumber, edge_select edge);


void enable_gpio_interrupt(uint32_t pinNumber,IRQn_Type irqNumber);

void clear_gpio_interrupt (uint32_t pinNumber);



#endif
