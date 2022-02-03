#include "stm32f3xx.h"                  // Device header
#include "drivers.h"

void clk_config(void){
	// PLLMUL <- 0x0E (PLL input clock x 16 --> (8 MHz / 2) * 16 = 64 MHz )  
	RCC->CFGR |= 0xE<<18;
	// Flash Latency, two wait states for 48<HCLK<=72 MHz
	FLASH->ACR |= 0x2;
	// PLLON <- 0x1 
  RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {};	
	// SW<-0x02 (PLL as System Clock), HCLK not divided, PPRE1<-0x4 (APB1 <- HCLK/2), APB2 not divided 
	RCC->CFGR |= 0x402;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
	SystemCoreClockUpdate();	
}


void ADC_config(void){
	//habilitar reloj a ADC1 y ADC2
	RCC->AHBENR |= RCC_AHBENR_ADC12EN + RCC_AHBENR_GPIOAEN + RCC_AHBENR_GPIOCEN;	
	
	//reset peripheral 
	RCC->AHBRSTR |=  RCC_AHBRSTR_ADC12RST;
	RCC->AHBRSTR &= ~RCC_AHBRSTR_ADC12RST;
	
	//configurar pines para entrada analogica (PA0,PC1)
	GPIOA->MODER |= (0x03<<0);
	GPIOC->MODER |= (0x03<<2);
	
	//Discontinous mode injected channel, Automatic injected group, overrun mode enable
	ADC1->CFGR |= ADC_CFGR_JAUTO + ADC_CFGR_JDISCEN + ADC_CFGR_OVRMOD;
	//2 conversions, Channel 7, channel 14
	ADC1->JSQR |= ADC_JSQR_JL_0 + (0x7<<8) + (0x1<<14) + ADC_JSQR_JEXTSEL_2 + ADC_JSQR_JEXTSEL_3 + ADC_JSQR_JEXTEN_0;
	//CLK/4
	ADC1_2_COMMON->CCR |= ADC12_CCR_CKMODE_0 | ADC12_CCR_CKMODE_1 | ADC12_CCR_VREFEN; 
	//Interrupt enable - End of Conversion Injected channel
	ADC1->IER |= ADC_IER_JEOC;
	//ADC Enable + ADC Start
	ADC1->CR |= ADC_CR_ADEN + ADC_CR_JADSTART;
	NVIC_EnableIRQ(ADC1_2_IRQn);
}

void UART3_config(void){
}

void GPIO_config(void){
	
}
