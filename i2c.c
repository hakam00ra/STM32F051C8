#include "i2c.h"
#include <stdint.h>





/**
  * @brief  I2C initialization
  * @param  I2C_TypeDef 
  */
void i2c_init (I2C_TypeDef *ic)
	{

	RCC->AHBENR|= 1<<18;  // gpio clock enable gpiob
	RCC->APB1ENR|= 1<<21; // enable i2c1 clock
	GPIOB->MODER|= 2<<12; // alternate function for two lines
	GPIOB->MODER|= 2<<14;
	GPIOB->AFR[0]|= (1<<24) | (1<<28); // i2c for alternate function
	ic->TIMINGR= 0x10420F0D;
	
	ic->CR1|= 1<<0;  // i2c enable
	for (int i=0;i<300000;i++);


}


/**
  * @brief  I2C continuous transmit an amount of data (up to 16 bytes) 
  * @param  ic pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for I2C module.
  * @param  Number of hbytes (high) to be transmitted
  * @param  Number of lbytes (low) to be transmitted
  * @param  Device ID
  * @param  Device address
  * @param  Data to be transmitted (15-8)
  * @param  Data to be transmitted (7-0)
  */
void i2c_transmit_cont (I2C_TypeDef *ic, uint8_t hbytes, uint8_t lbytes, uint8_t device, uint8_t address, uint64_t hdata, uint64_t ldata)
{	for (int i=0;i<300000;i++);
	
	uint8_t xldata[8]={ldata, ldata>>8, ldata>>16, ldata>>24, ldata>>32, ldata>>40, ldata>>48, ldata>>56};
	uint8_t xhdata[8]={hdata, hdata>>8, hdata>>16, hdata>>24, hdata>>32, hdata>>40, hdata>>48, hdata>>56};
	uint8_t bytes=hbytes+lbytes+1;
	
  ic->CR1|= 1<<0;  // i2c enable
  ic->CR2=0x0;
	ic->CR2|= 1<<25 | (bytes<<16) | (device<<1) ;  // write
	ic->CR2|= 1<<13;
  while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= address;
		for (int i=hbytes-1;i>=0;i--){
	while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= xhdata[i];  
	}
	if (lbytes>0){
		for (int i=lbytes-1;i>=0;i--){
	while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= xldata[i];  
			
	}
	}
	
	for (int i=0;i<300;i++);
ic->CR1&=~ 1<<0;  // i2c disable
}



/**
  * @brief  I2C transmit one byte
  * @param  ic pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for I2C module.
  * @param  Number of bytes to be transmitted
  * @param  Device ID
  * @param  Device address
  * @param  Data to be transmitted
  */
void i2c_transmit (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address, uint8_t data)
{	for (int i=0;i<300000;i++);
  ic->CR1|= 1<<0;  // i2c enable
  ic->CR2=0x0;
	ic->CR2|= (1<<25) | (bytes<<16) | (device<<1) ;  // write
	ic->CR2|= 1<<13;
  while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= address;
	while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= data;  
	for (int i=0;i<300;i++);
	ic->CR1&=~ 1<<0;  // i2c disable
}




/**
  * @brief  I2C receive one byte
  * @param  ic pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for I2C module.
  * @param  Number of bytes to be received
  * @param  Device ID
  * @param  Device address
  * @retval Data received  
  */
uint64_t i2c_receive (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address)
{	for (int i=0;i<30000;i++);
uint64_t result=0;
	ic->CR1|= 1<<0; // enable
//	ic->CR1&=~ I2C_CR1_PE ;
  ic->CR2=0x0;
	ic->CR2&=~ 1<<25;  // autoend disable
	ic->CR2|= (bytes<<16) | (device<<1) ;
	ic->CR2|= 1<<13;  // start
  while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= address;	
  while (!(I2C1->ISR & I2C_ISR_TC));	
	
	ic->CR2|= (1<<10) | (bytes<<16) | (device<<1) ;
	ic->CR2|= 1<<13;	// restart
	while (!(I2C1->ISR & I2C_ISR_RXNE));
	while (!(I2C1->ISR & I2C_ISR_TC));
  result=ic->RXDR;	
	ic->CR2|= 1<<14;	// stop
	for (int i=0;i<300;i++);
  ic->CR1&=~ 1<<0;
	return result;
}



uint64_t i2c_receive_cont (I2C_TypeDef *ic, uint8_t bytes, uint8_t device, uint8_t address, uint64_t *phdata, uint64_t *pldata)
{	for (int i=0;i<30000;i++);
  uint8_t hdata[8];
	uint8_t ldata[8];
	ic->CR1|= 1<<0; // enable
//	ic->CR1&=~ I2C_CR1_PE ;
  ic->CR2=0x0;
	ic->CR2&=~ 1<<25;  // autoend disable
	ic->CR2|= (1<<16) | (device<<1) ;
	ic->CR2|= 1<<13;  // start
  while (!(I2C1->ISR & I2C_ISR_TXIS));
	ic->TXDR= address;	
  while (!(I2C1->ISR & I2C_ISR_TC));	
		
	 ic->CR2=0x0;
	ic->CR2|= (1<<10) | (bytes<<16) | (device<<1) ;
	ic->CR2|= 1<<13;	// restart
	if (bytes>8){
	for (int i=8-1;i>=0;i--){
	while (!(I2C1->ISR & I2C_ISR_RXNE));
	//while (!(I2C1->ISR & I2C_ISR_TC));
	hdata[i]=ic->RXDR;}
  *phdata=hdata[7]<<56 | hdata[6]<<48 | hdata[5]<<40 | hdata[4]<<32 | hdata[3]<<24 | hdata[2]<<16 | hdata[1]<<8 | hdata[0];
	for (int i=bytes-8-1;i>=0;i--){
	while (!(I2C1->ISR & I2C_ISR_RXNE));
	//while (!(I2C1->ISR & I2C_ISR_TC));
	ldata[i]=ic->RXDR;}
  *pldata=ldata[7]<<56 | ldata[6]<<48 | ldata[5]<<40 | ldata[4]<<32 | ldata[3]<<24 | ldata[2]<<16 | ldata[1]<<8 | ldata[0];	
}
	else{
		for (int i=bytes-1;i>=0;i--){
	while (!(I2C1->ISR & I2C_ISR_RXNE));
	//while (!(I2C1->ISR & I2C_ISR_TC));
	hdata[i]=ic->RXDR;}
  *phdata=hdata[7]<<56 | hdata[6]<<48 | hdata[5]<<40 | hdata[4]<<32 | hdata[3]<<24 | hdata[2]<<16 | hdata[1]<<8 | hdata[0];
	}

	
	
	while (!(I2C1->ISR & I2C_ISR_TC));
	ic->CR2|= 1<<14;	// stop
	for (int i=0;i<300;i++);
  ic->CR1&=~ 1<<0;
	
}














