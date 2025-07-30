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

// Prototipos
void setup(void);
void mostrar_en_leds(uint8_t valor);

// Variables globales
uint8_t mostrar_leds = 0;
uint8_t valor_leds = 0;

int main(void)
{
	setup();

	uint16_t val1, val2;
	uint8_t comando;

	while (1)
	{
		// Leer valores de los potenciómetros
		val1 = adc_read(6);
		val2 = adc_read(7);

		// Ver si el maestro envía un comando
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
			valor_leds = spi_slave_receive();  // Guardar el nuevo valor
			mostrar_leds = 1;                  // Activar visualización
		}

		// Mantener LEDs encendidos
		if (mostrar_leds)
		{
			mostrar_en_leds(valor_leds);
		}
	}
}

void setup()
{
	cli();                  // Desactivar interrupciones
	adc_init();             // Inicializar ADC
	spi_slave_init();       // Inicializar SPI en modo esclavo

	DDRD = 0xFF;            // LEDs en PD0–PD7 como salida
	PORTD = 0x00;           // Apagar LEDs al inicio

	sei();                  // Activar interrupciones globales (por si se usan luego)
}

void mostrar_en_leds(uint8_t valor)
{
	PORTD = valor;          // Mostrar valor directamente en los LEDs
}







