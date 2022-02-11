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
uint8_t info1;
uint8_t flag_ready;
int n = 0;
int n1 = 0;
char * p1;
char * p2;
char buffer1[3000];
char buffer2[100];
uint8_t command_ready = 0;
uint8_t prueba = 0;
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
		if(command_ready){ //Comando AT LISTO 
			USART3_SENDSTR(p2); //Envio Dato Recibido a modulo WIFI
			command_ready = 0;   //COMMAND estado anterior 
		}
		if(flag_ready){
			USART2_SENDSTR(p1);
			flag_ready = 0;
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
		info1= USART3->RDR;
		if(info1 != '\r'){
			buffer1[n] = USART3->RDR;
			n++;
		} 
		else{
			buffer1[n] = '\r';
			buffer1[n+1] = '\n';
			buffer1[n+2] = '\0';
			n=0;
			flag_ready = 1;
			prueba++;
		}
	}
}

void USART2_IRQHandler(void){
	if(USART2->ISR & USART_ISR_RXNE){
		info= USART2->RDR;
		if(info != '\r'){
			buffer2[n1] = USART2->RDR;
			n1++;
		} else{
			
			buffer2[n1] = '\r';
			buffer2[n1+1] = '\n';
			buffer2[n1+2] = '\0';
			command_ready=1;
			n1=0;
		}
	}
}
/***************************************************************/
