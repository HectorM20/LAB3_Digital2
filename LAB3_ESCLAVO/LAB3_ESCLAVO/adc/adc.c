#include <avr/io.h>
#include "adc.h"

//Configuración del ADC
void adc_init(void){
	ADMUX = (1 << REFS0);				//Referencia a AVCC (5V)
	ADCSRA = (1 << ADEN)				//Habilitar ADC
	| (1 << ADPS2) | (1 << ADPS1);		//Prescaler de 64
} 

//Selección de canal
uint16_t adc_read(uint8_t canal){
	if (canal > 7) canal = 0;						//Canal ADC, entre ADC0-ADC7
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);		//Usa 4 bits 
	ADCSRA |= (1<< ADSC);							//Inicia conversión
	while (ADCSRA & (1 << ADSC));					//Espera a que termine la conversión
	return ADC;										//Decuelve el valor entre 0-1023	
}