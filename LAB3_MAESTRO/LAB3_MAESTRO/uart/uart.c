#include "uart.h"

void initUART()
{
	//Configurar PD0 (RX) como entrada y PD1 (TX) como salida
	DDRD |= (1 << DDD1);			//TX
	DDRD &= ~(1 << DDD0);			//RX

	//No usar doble velocidad
	UCSR0A = 0;

	//Habilitar receptor, transmisor e interrupción de recepción
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	//Configurar: 8 bits, sin paridad, 1 bit de parada
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

	//UBRR0 = 103 para 9600 baudios a 16 MHz (sin U2X0)
	UBRR0 = 103;
}

void UART_sendChar(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));		//Esperar buffer libre
	UDR0 = c;
}

void UART_sendString(char *texto)
{
	while (*texto)
	{
		UART_sendChar(*texto++);
	}
}
