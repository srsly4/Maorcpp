/*
 * Maor_12T_Custom_Firmware.cpp
 *
 * Created: 2013-12-28 23:20:58
 *  Author: Sirius
 */ 
#define F_CPU 16000000UL
#define TIMER0_PRESCALER 1024
#define TIMER0_IRQ_FREQUERENCY 100

#define __DELAY_BACKWARD_COMPATIBLE__


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdlib.h>
#include <avr/eeprom.h>

#include "ledcontroller.h"
#include "ADCController.h"
#include "MotorController.h"
#include "SonarController.h"
#include "UARTController.h"

LedController *leds;
ADCController *adc;
MotorController *motor;
SonarController *sonar;
UARTController *uart;

unsigned char sonarRefreshCounter = 0;

//WAZNE - definicje operatorow
void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	free(ptr);
}

void initialize_timer0()
{
	//Timer0 - od przerwania
	OCR0= (unsigned char)(((F_CPU/TIMER0_PRESCALER) / TIMER0_IRQ_FREQUERENCY) - 1);
	TCCR0 |= (1<<WGM01)|(1<<CS00)|(1<<CS02); //timer0 w trybie CTC, preskaler 1024
	TIMSK |= (1<<OCIE0); //przerwanie przy OC0 match
}

// Czujniki linii
#define LINLPWR 	       6
#define LINLPWR_DDR 	   DDRD
#define LINLPWR_PORT 	   PORTD
// przod-lewy
#define LINL_F 	           1
#define LINL_F_DDR 	       DDRB
#define LINL_F_PORT 	   PORTB
#define LINL_F_PIN 	       PINB
// przod-prawy
#define LINR_F 	           7
#define LINR_F_DDR 	       DDRD
#define LINR_F_PORT 	   PORTD
#define LINR_F_PIN 	       PIND
// tyl-lewy
#define LINL_B 	           0
#define LINL_B_DDR 	       DDRB
#define LINL_B_PORT 	   PORTB
#define LINL_B_PIN 	       PINB
// tyl-prawy
#define LINR_B 	           0
#define LINR_B_DDR 	       DDRC
#define LINR_B_PORT 	   PORTC
#define LINR_B_PIN 	       PINC

// Switch S1
#define S1 	               3
#define S1_DDR 	           DDRD
#define S1_PORT 	       PORTD
#define S1_PIN 	           PIND

// Sprawdzanie stanow czujniow linii
#define IS_LINL_F          (LINL_F_PIN&(1<<LINL_F))
#define IS_LINR_F          (LINR_F_PIN&(1<<LINR_F))
#define IS_LINL_B          (LINL_B_PIN&(1<<LINL_B))
#define IS_LINR_B          (LINR_B_PIN&(1<<LINR_B))

#define LINLPWR_ON         LINLPWR_PORT&=~(1<<LINLPWR)
#define LINLPWR_OFF        LINLPWR_PORT|=(1<<LINLPWR)

int main(void)
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2);  // wejscie INT0 dla kodu odbiornika RC5 z podciagnieciem
	
	DDRA |= (0x01 << 2);
	
	LINL_F_DDR&=~(1<<LINL_F); //TODO klasa czujnikow linii
	LINR_F_DDR&=~(1<<LINR_F);
	LINL_B_DDR&=~(1<<LINL_B);
	LINR_B_DDR&=~(1<<LINR_B);
	
	LINLPWR_DDR|=(1<<LINLPWR);
	LINLPWR_PORT|=(1<<LINLPWR); // wyl
	
	S1_PORT|=(1<<S1); // przycisk
	MCUCR|=(1<<ISC11); // opadajace zbocze na S1 generuje przerwanie
	GICR|=(1<<INT1);
	
	// Nieuzywane piny jako wyjscia
	DDRB|=(1<<PB2)|(1<<PB3)|(1<<PB4);
	
	leds = new LedController();
	leds->TurnOn(LED_FL);
	
	initialize_timer0();
	sonar = new SonarController();
	adc = new ADCController();
	motor = new MotorController(adc);
	leds->TurnOn(LED_FR);
	
	//uart = new UARTController();
	leds->TurnOn(LED_BL);
	
	sei(); //wlacz przerwania
	adc->startConversion();
	leds->TurnOn(LED_BR);
	
	_delay_ms(500);
	
	leds->TurnOff(LED_ALL);
	_delay_ms(250);
	leds->TurnOn(LED_ALL);
	_delay_ms(200);
	leds->TurnOff(LED_ALL);
	
	sonar->IsEnabled = 1;
	
	srand(12345);
	
    while(1)
    {
		/*LINLPWR_ON;
		_delay_us(50);
		char left = ((IS_LINL_F)? 0 : 1);
		LINLPWR_OFF;
		_delay_us(50);
		LINLPWR_ON;
		_delay_us(50);
		char right = ((IS_LINR_F)? 0 : 1);
		LINLPWR_OFF;
		if (left || right)
		{
			PORTA |= 0x01;
			motor->SetSpeed(-100, -100);
			_delay_ms(300);
			PORTA &= ~(0x01);
			motor->SetSpeed(100, -100);
			_delay_ms(((rand()%100 + 120)) * 4);
			motor->SetSpeed(0,0);
		}
		else
		{
			motor->SetSpeed(100, 100);
		}*/
		
		if (sonar->LeftSonar < 30)
			leds->TurnOn(LED_ALL);
		else
			leds->TurnOff(LED_ALL);
		
		/*PORTA |= (0x01 << 2);
		leds->TurnOn(LED_ALL);
		_delay_ms(1000);
		PORTA &=~(0x01 << 2);
		leds->TurnOff(LED_ALL);
		_delay_ms(1000);*/
		
    }
}


/*
* OBS£UGA PRZERWAÑ
*/

ISR(ADC_vect) //Przerwanie ADC Conversion Complete
{
	adc->event_conversion_completed();
}

ISR(TIMER0_COMP_vect) //Przerwanie CTC/overflow timera 0
{
	sonarRefreshCounter++;
	if (sonarRefreshCounter > 2) //niech tak nie zapierdala bo atmega nie wyrabia...
	{
		sonarRefreshCounter = 0;
		sonar->event_ReceiveSonarData();
	}
}

ISR(USART_UDRE_vect)
{
	uart->event_UartDataBufferEmpty();
}

ISR(USART_RXC_vect)
{
	uart->event_DataReceived();
}