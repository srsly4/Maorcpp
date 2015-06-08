/*
 * SonarController.h
 *
 * Created: 2013-12-30 17:24:43
 *  Author: Sirius
 */ 


#ifndef SONARCONTROLLER_H_
#define SONARCONTROLLER_H_

//Konfiguracja p³ytki
#define S_CS	           7
#define S_CS_DDR 	       DDRC
#define S_CS_PORT 	       PORTC
#define S_CS_PIN 	       PINC

#define SPIPORT PORTB
#define SPIPIN  PINB
#define SPIDDR  DDRB

#define SPISCK   PB7
#define SPIMISO  PB6
#define SPIMOSI  PB5
//#define SPISS    PB4

#define SPI_SS_HIGH() (S_CS_PORT |= (1<<S_CS))
#define SPI_SS_LOW() (S_CS_PORT &= ~(1<<S_CS))

#define SONAR_BUFFER_SIZE 5

class SonarController
{
	private:
		uint8_t spi_readwrite(uint8_t data);
		uint8_t spi_read(void);
		
		unsigned char leftSensorBuffer[SONAR_BUFFER_SIZE], rightSensorBuffer[SONAR_BUFFER_SIZE]; // bufory wartosci sonarow do usredniania ich wartosci
		volatile unsigned char bufferAlocated;
	public:
		volatile char IsEnabled;
		volatile unsigned char LeftSonar; // sensory z unormowana wielkoscia <- tych uzywac!
		volatile unsigned char RightSonar;
		volatile unsigned char VirtualSonar; // wirtualny sonar "srodkowy"
		
		volatile unsigned char QuickLeftSensor, QuickRightSensor; // wartosci sensorow stabilne jak windows 95 <- NIE UZYWAC!
		
		SonarController();
		void event_ReceiveSonarData(); // pobieranie danych z mikrokontrolera sonaru, funkcja wywo³ywana w przerwaniu timera
		
};



#endif /* SONARCONTROLLER_H_ */