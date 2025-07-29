//***************************************************************
//Universidad del Valle de Guatemala
//Electrónica Digital 2
//Autor: Héctor Alejandro Martínez Guerra
//Hardware: ATMEGA328P
//LAB3
//***************************************************************
//***************************************************************

//ESCLAVO
//ESCLAVO
//ESCLAVO

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "adc/adc.h"
#include "spi_slave/spi_slave.h"

//Prototipos
void setup(void);
void mostrar_en_leds(uint8_t valor);

int main(void)
{
	setup();

	uint16_t val1, val2;
	uint8_t comando;

	while (1)
	{
		val1 = adc_read(6);
		val2 = adc_read(7);

		comando = spi_slave_receive();

		if (comando == 0xAA)
		{
			SPDR = 0x55;

			spi_slave_receive(); SPDR = (val1 >> 8);
			spi_slave_receive(); SPDR = (val1 & 0xFF);
			spi_slave_receive(); SPDR = (val2 >> 8);
			spi_slave_receive(); SPDR = (val2 & 0xFF);
		}
		else if (comando == 0xCC)
		{
			uint8_t valor = spi_slave_receive();
			mostrar_en_leds(valor);
		}
	}
}

void setup()
{
	cli();
	adc_init();
	spi_slave_init();
	//PD0–PD7 como salida
	DDRD = 0xFF; 
	PORTD = 0x00;

	sei();
}

void mostrar_en_leds(uint8_t valor)
{
	PORTD = valor;
}






