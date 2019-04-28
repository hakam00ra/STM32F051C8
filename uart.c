#include "uart.h"
#include <stdint.h>


void uart_init(uint8_t uar){

 RCC->AHBENR|= 1<<17; // enable gpioa clock
switch (uar){
 
	case 1:
	{
		
		RCC->APB2ENR|= 1<<14; // enable uart1 clock
	  GPIOA->MODER &= ~ (3<<18);   // alternate function for gpios
    GPIOA->MODER |= 2<<18;	
		GPIOA->MODER &= ~ (3<<20);   
    GPIOA->MODER |= 2<<20;
		GPIOA->AFR[1] |= 0x1<<4;
		GPIOA->AFR[1] |= 0x1<<8;
    USART1->BRR = 0x681;
	  USART1->CR1 |= (1<<3) | (1<<0);
	  
	break;
	}
  
	case 2:{
		
		RCC->APB1ENR|= 1<<17; // enable uart2 clock
	  GPIOA->MODER &= ~ (3<<4);   // alternate function for gpios
    GPIOA->MODER |= 2<<4;	
		GPIOA->MODER &= ~ (3<<6);   
    GPIOA->MODER |= 2<<6;
    USART2->BRR = 0x681;
	  USART2->CR1 |= (1<<3) | (1<<0);
	break;
	}

}
}

void print(uint8_t uar, char*msg, ...){

char buff[80];
	
	va_list args;
	va_start(args,msg);
	vsprintf(buff,msg,args);
  
	for (int i=0;i<strlen(buff);i++)
	{
		if(uar==1){
	  USART1->TDR=buff[i];
		while (!(USART1->ISR &  USART_ISR_TXE));
		
		}
		if(uar==2){
		USART2->TDR=buff[i];
		while (!(USART2->ISR &  USART_ISR_TXE));
		
		}
	
	
	}
}











