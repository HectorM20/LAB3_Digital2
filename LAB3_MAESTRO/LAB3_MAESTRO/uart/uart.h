#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

//Prototipos de funciones
void initUART(void);
void UART_sendChar(char c);
void UART_sendString(char *texto);

#endif