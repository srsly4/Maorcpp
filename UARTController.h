/*
 * UARTController.h
 *
 * Created: 2013-12-30 19:05:56
 *  Author: Sirius
 */ 


#ifndef UARTCONTROLLER_H_
#define UARTCONTROLLER_H_

#define UART_TX_BUFFER_SIZE 8

class UARTController
{
	public:
		UARTController();
		
		void TransmitByte(unsigned char data);
		void TransmitData(unsigned char *data, unsigned char size);
		
		void event_UartDataBufferEmpty();
		void event_DataReceived();
		
	private:
		unsigned char transmitBuffer[8];
		unsigned char transmitBufferAllocated;
};



#endif /* UARTCONTROLLER_H_ */