#include "stm32f3xx.h"                  // Device header
#include "drivers.h"
/*
Proyecto Sistemas Embebidos 2022 
Fabián Hernández
Sistema de Irregacion con integracion de servidor web
*/

/****************Seccion Variables Globales******************/
/************************************************************/

int main(void){
	clk_config();					//Clock a 64MHz
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
/***************************************************************/
