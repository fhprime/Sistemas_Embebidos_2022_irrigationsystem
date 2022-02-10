#include "stm32f3xx.h"                  // Device header
#include "drivers.h"
/*
Proyecto Sistemas Embebidos 2022 
Fabián Hernández
Sistema de Irregacion con integracion de servidor web
*/

/****************Seccion Variables Globales******************/
float sensor_humedad_1;
float sensor_humedad_2;
uint8_t info;
uint8_t flag_ready;
uint8_t n = 0;
char * p1;
char * p2;
char buffer1[1000];
char buffer2[1000];
uint8_t command_ready = 0;
/************************************************************/

int main(void){
	p1 = buffer1;
	p2 = buffer2;
	clk_config();					//Clock a 64MHz
	TIM3_config();				//Configuramos TIM3 para trigger ADC
	ADC_config();					//Configuramos ADC (2 conversiones inyectadas)
	UART3_config();			//Comunicacion serial con Modulo WIFI 
	UART2_config();
	GPIO_config();				//Configuramos Puertos para Bomba de agua y electrovalvulas
	//PA10 RELE 1
	//PB3  RELE 2
	//PB5  Bomba  
	//PC1 Analog Measure Humidity sensor
	//PA0 Analog Measure Humidity sensor
	//PB8 UART3 RX
	//PB9 UART3 TX
	//PA2 UART2 TX
	//PA3 UART2 RX
	
	while(1){
	/*	if(buffer[0] == 0x31){  //Encender Electrovalvula 1 
			GPIOA->ODR |= (0x1<<10);
		}
		if(buffer[0] == 0x32){  //Apagar Electrovalvula 1
			GPIOA->ODR &= ~(0x1<<10);
		}
		if(buffer[0] ==  0x33){	//Encender Electrovalvula 2
			GPIOB->ODR |= (0x1<<3);
		}
		if(buffer[0] == 0x34){ 	//Apagar Electrovalvula 2 
			GPIOB->ODR &= ~(0x1<<3);
		}
		if(buffer[0] == 0x35){	//Encender Bomba 
			GPIOB->ODR |= (0x1<<5);
		}
		if(buffer[0] == 0x36){	//Apagar Bomba
			GPIOB->ODR &= ~(0x1<<5);
		}*/
		if(command_ready){
			USART3_SENDSTR(p2);
		}
		if(flag_ready){
			USART2_SENDSTR(p1);
		
		}
	}
}


//Miercoles Prueba Humedad y Electrovalvulas
//Jueves Prueba Modulo WIFI
//Viernes Prueba Servidor WEB 

/************Seccion Rutinas de Servicio de Interrupcion********/
void ADC1_2_IRQHandler(void){
	//A mayor humedad menor voltaje 
	sensor_humedad_1 = (float)((ADC1->JDR1*3.3f)/4095.0f);
	sensor_humedad_2 = (float)((ADC1->JDR2*3.3f)/4095.0f);
}

void USART3_IRQHandler(void){
	if(USART3->ISR & USART_ISR_RXNE){
		info= USART3->RDR;
		if(info != '\r'){
			buffer1[n] = USART3->RDR;
			n++;
			flag_ready = 0;
		} 
		else{
			buffer1[n] = '\0';
			n=0;
			flag_ready = 1;
		}
	}
}

void USART2_IRQHandler(void){
	if(USART2->ISR & USART_ISR_RXNE){
		info= USART2->RDR;
		if(info != '\r'){
			buffer2[n] = USART2->RDR;
			n++;
			command_ready = 0;
		} else{
			buffer2[n] = '\0';
			command_ready=1;
			n=0;
		}
	}
}
/***************************************************************/
