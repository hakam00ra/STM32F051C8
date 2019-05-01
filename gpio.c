#include "gpio.h"
#include <stdint.h>



/**
  * @brief  GPIO initialization
  * @param  user data from typedef struct in main
  */
void gpio_init(GPIO_TYPE gpio_type){

	if (gpio_type.gp==GPIOA)
	RCC->AHBENR|= 1<<17;  // gpio clock enable
  if (gpio_type.gp==GPIOB)
	RCC->AHBENR|= 1<<18;
  if (gpio_type.gp==GPIOC)
	RCC->AHBENR|= 1<<19;
	
	
                gpio_type.gp->MODER&= ~ 2<<(gpio_type.pin*2);  // gpio mode
                gpio_type.gp->MODER|=  gpio_type.mode<<(gpio_type.pin*2);
	
	if (gpio_type.mode==output_mode && gpio_type.type==pp ){
		gpio_type.gp->OTYPER&=~ 1<<gpio_type.pin;
	        gpio_type.gp->OSPEEDR&=~3<<(gpio_type.pin*2);	
		gpio_type.gp->OSPEEDR|= gpio_type.speed<<(gpio_type.pin*2);	
	
}
        if (gpio_type.mode==output_mode && gpio_type.type==od){ 
		gpio_type.gp->OTYPER|= 1<<gpio_type.pin;
		gpio_type.gp->OSPEEDR&=~3<<(gpio_type.pin*2);	
		gpio_type.gp->OSPEEDR|= gpio_type.speed<<(gpio_type.pin*2);	
}
  
	if (gpio_type.mode==input_mode) {  // resistors pull up and pull down
		gpio_type.gp->PUPDR&=~3<<(gpio_type.pin*2);	
		gpio_type.gp->PUPDR|= gpio_type.res<<(gpio_type.pin*2);	
	}

        if (gpio_type.mode==alt_func && gpio_type.pin<8) {  // alternate functions low and high registers
                gpio_type.gp->AFR[0]|= gpio_type.af<<(gpio_type.pin*4);	
	}
		
        if (gpio_type.mode==alt_func && gpio_type.pin>7) {
                gpio_type.gp->AFR[1]|= gpio_type.af<<(gpio_type.pin*4);	
	}

	
}


/**
  * @brief  GPIO write 1 or 0 
  * @param  gp pointer to a GPIO_HandleTypeDef structure that contains
  *               the configuration information for GPIO module.
  * @param  GPIO pin number
  * @param  State of GPIO pin (1 or 0)
  */
void gpio_write(GPIO_TypeDef *gp, uint32_t pinNumber, uint8_t state){

	if (state==0)
gp->BSRR|=1<<(pinNumber+16);

	if (state==1)
gp->BSRR|=1<<(pinNumber);
}


/**
  * @brief  GPIO toggle status
  * @param  gp pointer to a GPIO_HandleTypeDef structure that contains
  *               the configuration information for GPIO module.
  * @param  GPIO pin number
  */
void gpio_toggle(GPIO_TypeDef *gp, uint32_t pinNumber){


gp->ODR ^=1<<pinNumber;

}


/**
  * @brief  GPIO interrupt configuration
  * @param  gp pointer to a GPIO_HandleTypeDef structure that contains
  *               the configuration information for GPIO module.
  * @param  GPIO pin number
  * @param Select the edge from which the interrupt is triggered
  */
void configure_gpio_interrupt(GPIO_TypeDef*gp, uint32_t pinNumber, edge_select edge){

RCC->APB2ENR|= 1<<0;  // enable peripheral clock for SYSCFG 
	
	if (gp==GPIOA)
	{
	switch(pinNumber){  // external interrupt configuration register selection for corresponding pin
	
		case 0:
			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI0_PA ;
		break;
		case 1:
			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI1_PA ;
    break;
		case 2:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI2_PA ;
    break;
		case 3:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI3_PA ;
    break;
		case 4:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI4_PA ;
		break;
		case 5:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI5_PA ;
    break;
		case 6:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI6_PA ;
    break;
		case 7:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI7_PA ;
    break;
		case 8:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI8_PA ;
		break;
		case 9:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI9_PA ;
    break;
		case 10:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI10_PA ;
    break;
		case 11:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI11_PA ;
    break;
		case 12:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI12_PA ;
		break;
		case 13:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI13_PA ;
    break;
		case 14:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI14_PA ;
    break;
		case 15:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI15_PA ;
    break;
	
	
	}
	
	
	}

	if (gp==GPIOB)
	{
	switch(pinNumber){
	
		case 0:
			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI0_PB ;
		break;
		case 1:
			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI1_PB ;
    break;
		case 2:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI2_PB ;
    break;
		case 3:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI3_PB ;
    break;
		case 4:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI4_PB ;
		break;
		case 5:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI5_PB ;
    break;
		case 6:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI6_PB ;
    break;
		case 7:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI7_PB ;
    break;
		case 8:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI8_PB ;
		break;
		case 9:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI9_PB ;
    break;
		case 10:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI10_PB ;
    break;
		case 11:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI11_PB ;
    break;
		case 12:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI12_PB ;
		break;
		case 13:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI13_PB ;
    break;
		case 14:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI14_PB ;
    break;
		case 15:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI15_PB ;
    break;
	
	
	}
	
	
	}
		if (gp==GPIOC)
	{
	switch(pinNumber){
	
		case 0:
		       	        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI0_PC ;
		break;
		case 1:
			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI1_PC ;
    break;
		case 2:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI2_PC ;
    break;
		case 3:
   			        SYSCFG->EXTICR[0]=SYSCFG_EXTICR1_EXTI3_PC ;
    break;
		case 4:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI4_PC ;
		break;
		case 5:
				SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI5_PC ;
    break;
		case 6:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI6_PC ;
    break;
		case 7:
   			        SYSCFG->EXTICR[1]=SYSCFG_EXTICR2_EXTI7_PC ;
    break;
		case 8:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI8_PC ;
		break;
		case 9:
				SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI9_PC ;
    break;
		case 10:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI10_PC ;
    break;
		case 11:
   			        SYSCFG->EXTICR[2]=SYSCFG_EXTICR3_EXTI11_PC ;
    break;
		case 12:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI12_PC ;
		break;
		case 13:
				SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI13_PC ;
    break;
		case 14:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI14_PC ;
    break;
		case 15:
   			        SYSCFG->EXTICR[3]=SYSCFG_EXTICR4_EXTI15_PC ;
    break;
	
	
	}
	
	
	}

	
		
	if (edge==RISING_EDGE)  // selection of edge detection
		EXTI->RTSR|= 1<<pinNumber;
	if (edge==FALLING_EDGE)
		EXTI->FTSR|= 1<<pinNumber;
	if (edge==RISING_FALLING_EDGE){
		EXTI->RTSR|= 1<<pinNumber;
EXTI->FTSR|= 1<<pinNumber;
}
}
	
	
/**
  * @brief  Enable GPIO interrupt
  * @param  Pin number
  * @param  irqNumber
  */	
void enable_gpio_interrupt(uint32_t pinNumber,IRQn_Type irqNUmber){

EXTI->IMR|= 1<<pinNumber;

NVIC_EnableIRQ(irqNUmber);


}

/**
  * @brief  Clear GPIO interrupt
  * @param  Pin number
  */	
void clear_gpio_interrupt (uint32_t pinNumber){

EXTI->PR|= 1<<pinNumber;

}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
