/*
 * SonarController.cpp
 *
 * Created: 2013-12-30 17:44:08
 *  Author: Sirius
 */ 
#include <avr/io.h>
#include "SonarController.h"

#define F_CPU 16000000UL
#include <util/delay.h>
#include <string.h>

#define IRQ_DELAY _delay_us(5)
#define TRANSFER_DATA (0x00)

 SonarController::SonarController()
{
	this->LeftSonar = 0;
	this->RightSonar = 0;
	this->VirtualSonar = 0;
	this->QuickLeftSensor = 0;
	this->QuickRightSensor = 0;
	this->bufferAlocated = 0;
	this->IsEnabled = 0;
	
	//INICJALIZACJA SPI
	S_CS_DDR|=(1<<S_CS); // pin CS jako wyjscie
	// SCK, SS!!, MOSI jak wyjscia
	SPIDDR |= (1<<SPISCK);
	// MISO jako wejscie
	SPIDDR &= ~(1<<SPIMISO);
	// Inicjalizacja, preskaler 128
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
	SPCR = 0;
	
}

uint8_t SonarController::spi_readwrite(uint8_t data)
{
	SPDR = data;
	// Czekaj na koniec transmisji
	while(!(SPSR & (1<<SPIF)));
	// Zwroc odebrana wartosc
	return SPDR;
}

uint8_t SonarController::spi_read(void)
{
	return spi_readwrite(TRANSFER_DATA);
}

void SonarController::event_ReceiveSonarData()
{
	if (this->IsEnabled == 1)
	{
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);  // SPI ON
	
		SPI_SS_LOW();   // nozka CS w stan niski
		IRQ_DELAY;
		this->QuickLeftSensor =  spi_read();
		IRQ_DELAY;
		this->QuickRightSensor = spi_read();
		SPI_SS_HIGH();  // nozka CS w stan wysoki
		SPCR = 0;  //SPI OFF
	
		//this->VirtualSonar = (this->LeftSonar + this->RightSonar) / 2;
	
		//uaktualnij usrednione sonary [ZA DUZO TAKTOW PROCESORA!!!]
		if (this->bufferAlocated >= 5)
		{
			memmove(this->leftSensorBuffer, (this->leftSensorBuffer) + 1, SONAR_BUFFER_SIZE - 1);
			memmove(this->rightSensorBuffer, (this->rightSensorBuffer) + 1, SONAR_BUFFER_SIZE - 1);
			
			this->leftSensorBuffer[SONAR_BUFFER_SIZE - 1] = this->QuickLeftSensor;
			this->rightSensorBuffer[SONAR_BUFFER_SIZE - 1] = this->QuickRightSensor;
			
			int tmp_sum_left = 0, tmp_sum_right = 0;
			for (unsigned char i = 0; i < SONAR_BUFFER_SIZE; i++)
			{
				tmp_sum_left += (int)(this->leftSensorBuffer[i]);
				tmp_sum_right += (int)(this->rightSensorBuffer[i]);
			}
			
			this->LeftSonar = (unsigned char)(tmp_sum_left / SONAR_BUFFER_SIZE);
			this->RightSonar = (unsigned char)(tmp_sum_right / SONAR_BUFFER_SIZE);
			this->VirtualSonar = (unsigned char)((this->LeftSonar + this->RightSonar)/2);
		}
		else
		{
			this->bufferAlocated++;
			this->LeftSonar = this->QuickLeftSensor;
			this->RightSonar = this->QuickRightSensor;
		}
	}
}
