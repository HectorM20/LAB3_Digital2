#include <avr/io.h>
#include "spi_slave.h"

void spi_slave_init(void){
	//Configurar MISO como salida 
	DDRB |= (1 << PB4);									//MISO
	DDRB &= ~((1 << PB3) | (1 << PB5) | (1 << PB2));	//MOSI, SCK Y SS como entrada
	//Habilitar SPI en modo esclavo
	SPCR = (1 << SPE);			
}

uint8_t spi_slave_receive(void){
	while (!(SPSR & (1 << SPIF)));						//Espera a que se reciba un dato
	return SPDR;										//Devuelve el dato recibido
}