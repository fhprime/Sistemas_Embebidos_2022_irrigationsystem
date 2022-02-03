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
/************************************************************/

int main(void){
	clk_config();					//Clock a 64MHz
	TIM3_config();				//Configuramos TIM3 para trigger ADC
	ADC_config();					//Configuramos ADC (2 conversiones inyectadas)
	UART3_config();			//Comunicacion serial con Modulo WIFI 
	GPIO_config();				//Configuramos Puertos para Bomba de agua y electrovalvulas
	
	
	while(1){
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
/***************************************************************/
