//***************************************************************
//Universidad del Valle de Guatemala
//Electrónica Digital 2
//Autor: Héctor Alejandro Martínez Guerra
//Hardware: ATMEGA328P
//LAB3
//***************************************************************
//***************************************************************

//MAESTRO 
//MAESTRO
//MAESTRO

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>     
#include <avr/interrupt.h>

#include "uart/uart.h"
#include "spi_master/spi_master.h"

//Prototipos
void setup(void);
void uart_send_values(uint16_t val1, uint16_t val2);
void mostrar_en_leds(uint8_t valor);
void esperar_a_esclavo_listo(void);

//Variables globales
#define UART_BUFFER_SIZE 5
volatile char uart_buffer[UART_BUFFER_SIZE];
volatile uint8_t uart_index = 0;

volatile uint8_t nuevo_dato_uart = 0;
volatile uint8_t valor_uart = 0;

int main(void)
{
	setup();

	uint8_t high1, low1, high2, low2;
	uint16_t val1, val2;

	while (1)
	{
		esperar_a_esclavo_listo();					//Maestro envía 0xAA, espera 0x55
		PORTB &= ~(1 << PB2);						//SS LOW: activa esclavo
		high1 = spi_master_transmit(0x00);			//Lee val1 (parte alta)
		low1  = spi_master_transmit(0x00);			//Lee val1 (parte baja)
		high2 = spi_master_transmit(0x00);			//val2 alta
		low2  = spi_master_transmit(0x00);			//val2 baja
		PORTB |= (1 << PB2);						//SS HIGH: desactiva esclavo

		val1 = (high1 << 8) | low1;
		val2 = (high2 << 8) | low2;

		uart_send_values(val1, val2);

		//Si llegó un nuevo valor por UART
		if (nuevo_dato_uart)
		{
			nuevo_dato_uart = 0;

			//Mostrar en LEDs
			mostrar_en_leds(valor_uart);

			//Confirmación
			char confirm[30];
			sprintf(confirm, "Recibido: %u\r\n", valor_uart);
			UART_sendString(confirm);

			//Enviar al esclavo
			PORTB &= ~(1 << PB2);
			spi_master_transmit(0xCC);         //comando LED
			spi_master_transmit(valor_uart);   //valor real
			PORTB |= (1 << PB2);
		}

		_delay_ms(500);
	}
}

void setup()
{
	cli();
	initUART();
	spi_master_init();

	//Desactivar I2C para liberar PC4 y PC5
	TWCR &= ~(1 << TWEN);

	//SS como salida
	DDRB |= (1 << PB2);
	PORTB |= (1 << PB2);

	//LEDs: PD3, PD2, PC0-PC5
	DDRD |= (1 << PD3) | (1 << PD2);
	DDRC |= 0b00111111;

	sei();
}

void uart_send_values(uint16_t val1, uint16_t val2)
{
	char buffer[50];
	sprintf(buffer, "Pot1: %u\t Pot2: %u\r\n", val1, val2);
	UART_sendString(buffer);
}

void esperar_a_esclavo_listo(void)
{
	uint8_t respuesta = 0;
	do {
		PORTB &= ~(1 << PB2);
		respuesta = spi_master_transmit(0xAA);
		PORTB |= (1 << PB2);
		_delay_ms(10);
	} while (respuesta != 0x55);
}

void mostrar_en_leds(uint8_t valor)
{
	//LED[7:0] = {PD3, PD2, PC5–PC0}
	PORTC = (PORTC & 0b11000000) | (valor & 0b00111111);

	if (valor & (1 << 6))
	PORTD |= (1 << PD2);
	else
	PORTD &= ~(1 << PD2);

	if (valor & (1 << 7))
	PORTD |= (1 << PD3);
	else
	PORTD &= ~(1 << PD3);
}

//Interrupción UART RX - recibe caracteres en forma de número (ej. "123\r")
ISR(USART_RX_vect)
{
	char c = UDR0;

	if (c == '\r') {
		uart_buffer[uart_index] = '\0';
		valor_uart = atoi((char*)uart_buffer);  //Convertir a número
		if (valor_uart <= 255) {
			nuevo_dato_uart = 1;
		}
		uart_index = 0;
		} else if (uart_index < UART_BUFFER_SIZE - 1) {
		uart_buffer[uart_index++] = c;
	}
}



