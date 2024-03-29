#include "spi.h"
#include <stdint.h>


/**
  * @brief  SPI initialization
  * @param  user data from typedef struct in main
  */
void spi_init(SPI_TYPE spi_type)
{
	if (spi_type.sp==SPI1) {
		RCC->AHBENR|= 1<<17;  // GPIOA ENABLE CLOCK
		RCC->APB2ENR|= 1<<12;  // SPI1 ENABLE CLOCK
		// should be configured for push pull but it is so by default
		GPIOA->MODER &= ~ (3<<10);  // alternate functions for GPIO 
		GPIOA->MODER |= 2<<10;	

		GPIOA->MODER &= ~ (3<<12);   
		GPIOA->MODER |= 2<<12;	

		GPIOA->MODER &= ~ (3<<14);   
		GPIOA->MODER |= 2<<14;
	
	}
		
		
 	if (spi_type.sp==SPI2) {
		RCC->AHBENR|= 1<<18;  // GPIOB ENABLE CLOCK
		RCC->APB1ENR|= 1<<14;  // SPI2 ENABLE CLOCK


		GPIOB->MODER &= ~ (3<<26);   // alternate fucntion for gpio (mosi, miso, sck)
		GPIOB->MODER |= 2<<26;	

		GPIOB->MODER &= ~ (3<<28);   
		GPIOB->MODER |= 2<<28;	

		GPIOB->MODER &= ~ (3<<30);   
		GPIOB->MODER |= 2<<30;
	}
	
	// spi alternate function code is zero, so no changes in the gpio afr register needed

	//  configuration
	if (spi_type.mas==master)
		spi_type.sp->CR1|= 1<<2;
	// polarity and phase
	if (spi_type.pol==pol1)
		spi_type.sp->CR1|= 1<<1;
	if (spi_type.ph==ph1)
		spi_type.sp->CR1|= 1<<0;

	// data size
	if (spi_type.bit==bits8) {
		spi_type.sp->CR2&=~ (0xF<<8);
	  	spi_type.sp->CR2|= 0x7<<8; // this if is unecessary as the register's reset value is 0x700 (8bit data size by default)
	}
	if (spi_type.bit==bits16)
		{spi_type.sp->CR2&=~ (0xF<<8);
	  	 spi_type.sp->CR2|= 0xF<<8; 
	}

 	 if (spi_type.fifo==th8)  // fifo threshold according to data size
		spi_type.sp->CR2|= (1<<12) | (1<<2);
}



/**
  * @brief  SPI transmit an amount of data 
  * @param  sp pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  Number of bytes to be transmitted
  * @param  Data to be transmitted
  */
void spi_transmit(SPI_TypeDef*sp, uint32_t bytes,uint32_t data){

	sp->CR1 |= (1<<6) | (1 << 9) | (1<<8);  // enable spi
	//	sp->CR2 |= 1<<2;
	
	uint8_t xdata[4]={data, data>>8, data>>16, data>>24};
	for (int i=bytes-1;i>=0;i--) {
		while ( (sp->SR & SPI_SR_TXE  ) == 0 && ( sp->SR & SPI_SR_BSY)==1);
		*((uint8_t*)&sp->DR) = (uint8_t) xdata[i];
	}
	
	// wait until FTLVL[1:0]=0
	while ( (sp->SR & SPI_SR_TXE  ) == 0 && ( sp->SR & SPI_SR_BSY)==1);
	while ((sp->SR & SPI_SR_FTLVL) ==1){sp->DR;};
	while(!(sp->SR & SPI_SR_RXNE)){sp->DR;};
		
  	while((sp->SR & SPI_SR_FRLVL )!=0) {
  		sp->DR;
	}

	//sp->CR1 &=~ (1 << 6); // disable spi
}


/**
  * @brief  SPI receive an amount of data 
  * @param  sp pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  Number of bytes to be received
  * @retval Data received
  */
uint64_t spi_receive(SPI_TypeDef*sp, uint32_t bytes)
{
	uint8_t spi_data[8];
	uint64_t      result=0;	

	sp->CR1 |= (1 << 6);  // enable spi
	
       for (int i=bytes-1;i>=0;i--) {
		while ( (sp->SR & SPI_SR_TXE  ) == 0 && ( sp->SR & SPI_SR_BSY)==1);
		*((uint8_t*)&sp->DR) = (uint8_t) 0x00;
		while(!(sp->SR & SPI_SR_RXNE));
		spi_data[i] = sp->DR;
			   
	}
	result= spi_data[5]<<40 | spi_data[4]<<32|spi_data[3]<<24 | spi_data[2]<<16 | spi_data[1]<<8 | spi_data[0];
	
	

	// read rx fifo until it's empty
 	
	while((sp->SR & SPI_SR_FRLVL )!=0){
		sp->DR;
	}
	//	sp->CR1 &=~ (1 << 6); // disable spi

	return result;
}








