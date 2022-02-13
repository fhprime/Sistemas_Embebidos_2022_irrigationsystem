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
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;					//Habilito reloj a GPIOB
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;				//Habilito reloj a USART3
	
	GPIOB->MODER |= (0x2<<16)| (0x2<<18);					//Configuro puerto PB8 y PB8 
	GPIOB->AFR[1] |= (0x7<<0); 									//Puerto PB8 TX
	GPIOB->AFR[1] |= (0x7<<4);									//Puerto PB9 RX
	
	USART3->BRR = 278;         									//(64MHz/2)/115200= 277,77
	USART3->CR1 |= USART_CR1_RE | USART_CR1_TE; //Habilito recepcion y transmision de datos
	USART3->CR1 |= USART_CR1_UE;								//Habilito USART3
	USART3->CR1 |= USART_CR1_RXNEIE;						//Habilito receptor de interrupciones
	NVIC_EnableIRQ(USART3_IRQn);                //Habilito NVIC para interrupcion al USART3
}

void GPIO_config(void){
	//PA10 RELE 1
	//PB3  RELE 2
	//PB5  Bomba 
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN + RCC_AHBENR_GPIOBEN;		//Activamos Reloj a Puertos A y B 
	GPIOA->MODER |= (0x1<<20);   //Configuramos puertos como Output
	GPIOB->MODER |= (0x1<<6) + (0x1<<10);
	GPIOB->MODER &= ~(0x1<<7);

}

void TIM3_config(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// Freq = 1kHz (999) - Autoreload register in us (333 is 333 us)
	TIM3->ARR = 999;
	// Prescaler (64MHz)/(63+1) = 1 MHz
	TIM3->PSC = 63; 
	// The update event is selected as trigger output (TRGO)
	TIM3->CR2 |= 0x20;
	// Update Generation
	TIM3->EGR |= TIM_EGR_UG;
	// Autoreload register + Counter Enable
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void USART3_SENDchar(uint8_t c){
	while(!(USART3->ISR & USART_ISR_TC));
	USART3->TDR = c;
}

void USART3_SENDSTR(char *string){
	while(*string){
		USART3_SENDchar(*string);
		string++;
	}
}

void UART2_config(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;					//Habilito reloj a GPIOA
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;				//Habilito reloj a USART2
	
	GPIOA->MODER |= (0x2<<4)| (0x2<<6);					//Configuro puerto A2 y A3 
	GPIOA->AFR[0] |= (0x7<<8); 									//Puerto A2 TX
	GPIOA->AFR[0] |= (0x7<<12);									//Puerto A3 RX
	
	USART2->BRR = 278;         									//(64MHz/2)/115200= 277,77
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE; //Habilito recepcion y transmision de datos
	USART2->CR1 |= USART_CR1_UE;								//Habilito USART2
	USART2->CR1 |= USART_CR1_RXNEIE;						//Habilito receptor de interrupciones
	NVIC_EnableIRQ(USART2_IRQn);                //Habilito NVIC para interrupcion al USART2
}

//Funcion para enviar un caracter 
void USART2_SENDchar(uint8_t c){
	while(!(USART2->ISR & USART_ISR_TC));
	USART2->TDR = c;
}
//Funcion para enviar un string
void USART2_SENDSTR(char *string){
	while(*string){
		USART2_SENDchar(*string);
		string++;
	}
}
