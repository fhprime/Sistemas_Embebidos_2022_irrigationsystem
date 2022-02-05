#ifndef DRIVERS_H_
#define DRIVERS_H_

void clk_config(void);
void ADC_config(void);					
void UART3_config(void);			
void GPIO_config(void);	
void TIM3_config(void);

void USART3_SENDchar(uint8_t c);
void USART3_SENDSTR(char *string);

#endif /* DRIVERS_H_ */
