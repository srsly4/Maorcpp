/*
 * UARTController.cpp
 *
 * Created: 2013-12-30 19:06:12
 *  Author: Sirius
 */
#define F_CPU 16000000UL
#include "UARTController.h"
#include <avr/io.h>
#include <string.h>


 UARTController::UARTController()
{
	this->transmitBufferAllocated = 0;
	
	#define BAUD 9600 //predkosc transmisji
	#include <util/setbaud.h> 
	
	//obliczone wartosci
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	
	//wlacz UART
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE); //transmisja i odbieranie wlaczone
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); // 8 bitowa ramka 1 bit stopu
}

void UARTController::event_UartDataBufferEmpty()
{
	if (this->transmitBufferAllocated > 0)
	{
		UDR = this->transmitBuffer[0]; // wyslij
		
		this->transmitBufferAllocated--; // kolejka FIFO
		memmove(this->transmitBuffer, (this->transmitBuffer) + 1, UART_TX_BUFFER_SIZE - 1);
		
		if (this->transmitBufferAllocated == 0) // gdy bufor pusty wylacz przerwanie
			UCSRB &= ~(1<<UDRIE);
	}
}

void UARTController::TransmitByte( unsigned char data )
{
	if (transmitBufferAllocated < UART_TX_BUFFER_SIZE) // nie ma miejsca to pakiet leci w pizdu... (nie trzeba tego naprawiac ;) )
	{
		this->transmitBuffer[this->transmitBufferAllocated++] = data;
		UCSRB |= (1<<UDRIE); 
	}
	
}

void UARTController::TransmitData( unsigned char *data, unsigned char size )
{
	for(unsigned char i = 0; i < size; i++)
	{
		this->TransmitByte(*(data++));
	}
}

void UARTController::event_DataReceived()
{
	
}

