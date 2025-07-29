#include <avr/io.h>
#include "spi_master.h"

//PB2 -> SS
//PB3 -> MOSI 
//PB4 -> MISO
//PB5 -> SCK
void spi_master_init(void) {
	//Configurar MOSI, SCK y SS como salida; MISO como entrada
	DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);		//MOSI, SCK, SS
	DDRB &= ~(1 << PB4);								//MISO
	
	//Habilitar SPI en modo maestro, velocidad fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_master_transmit(uint8_t data) {
	SPDR = data;									//Cargar dato al registro de transmisión
	while (!(SPSR & (1 << SPIF)));					//Esperar a que se complete
	return SPDR;									//Devolver el byte recibido
}
