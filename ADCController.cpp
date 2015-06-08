/*
 * ADCController.cpp
 *
 * Created: 2013-12-29 00:42:16
 *  Author: Sirius
 */ 

#include "ADCController.h"
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



void ADCController::startConversion( void )
{
	ADCSRA|=(1<<ADSC);
}

void ADCController::stopConversion( void )
{
	ADCSRA&=~(1<<ADSC);
}

void ADCController::turnOff( void )
{
	ADCSRA &= ~(1<<ADEN);
}

void ADCController::turnOn( void )
{
	ADCSRA |= (1<<ADEN);
}

void ADCController::selectChannel( unsigned char channel )
{
	ADMUX &= 0b11111000;
	ADMUX |= channel;
}

unsigned int ADCController::getOffsetError( void )
{
	unsigned int offset = 0, i;
	
	ADMUX |= (1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); //wybranie GND jako napiecia do pomiaru
	_delay_ms(1); //czas na ustabilizowanie sie napiecia

	this->startConversion();
	for(i=0;i<ADC_MEASUREMENTS_OFFSET;i++) //usrednienie wyniku
	{
		this->waitForConversionComplete(); //czekanie na zakonczenie konwersji
		offset += ADC;
	}
	///ADC_stop_conversion();
	offset =offset / ADC_MEASUREMENTS_OFFSET;
	ADMUX &= ~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
	return offset;
}

void ADCController::waitForConversionComplete( void )
{
	while(!(ADCSRA & (1<<ADIF))); //Stan flagi ADIF jest niski dopoki ADC nie skonczy konwersji
	ADCSRA |= (1<<ADIF); 	//wyzerowanie flagi przerwania aby przy nastepnym wywolaniu
	//nie zostala odczytana flaga z poprzedniej konwersji
}

 ADCController::ADCController()
{
	this->measure = 0;
	this->adcChannel = 0;
	this->measurement_count = ADC_MEASUREMENTS_NUMBER;
	
	ADMUX = (1<<REFS0)|(1<<REFS1) | 0b01101 ;
	ADCSRA|=(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//preskaler 128 - 125kHz@16MHz, wlaczone przerwania, konwersja caigla
}

void ADCController::event_conversion_completed()
{
	switch(this->adcChannel)
	{
		case 0:  // kanal 0
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik konwersji nie bedzie dotyczyl poprzedniej wielkosci mierzonej
				this->adcChannel = 1;
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00001;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->ADC0Measurement = this->measure / ADC_MEASUREMENTS_NUMBER; //srednia z pomiarow
				this->measure=0;
				
				this->turnOn();
				this->startConversion();
			}
			
		}
		break;
		
		case 1:  // kanal 1
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu
				this->adcChannel = 2;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00010 ;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->ADC1Measurement = this->measure / ADC_MEASUREMENTS_NUMBER; //srednia z pomiarow
				this->measure=0;
				
				this->turnOn();
				this->startConversion();
			}
			
		}
		break;
		
		case 2:  // kanal 2
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu
				this->adcChannel = 3;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00011 ;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->ADC2Measurement = this->measure / ADC_MEASUREMENTS_NUMBER;
				this->measure = 0;
				
				this->turnOn();
				this->startConversion();
			}
		}
		break;
		
		case 3:  // kanal 3
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu
				this->adcChannel = 4;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00100 ;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->BatteryADC = this->measure / ADC_MEASUREMENTS_NUMBER;
				this->measure = 0;
				
				this->turnOn();
				this->startConversion();
			}
		}
		break;
		
		case 4:  // kanal 4
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu
				this->adcChannel = 5;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00101 ;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->Motor1Current = this->measure / ADC_MEASUREMENTS_NUMBER;
				this->measure = 0;
				
				this->turnOn();
				this->startConversion();
			}
		}
		break;
		
		case 5:  // kanal 5
		{
			this->measure += ADC;
			if(--this->measurement_count == 0)
			{
				this->turnOff(); //ma na celu upewnienie sie zenastepny wynik nie bedzi dotyczyl poprzedniego kanalu
				this->adcChannel = 0;
				
				ADMUX = (1<<REFS0)|(1<<REFS1) | 0b00000 ;
				
				this->measurement_count=ADC_MEASUREMENTS_NUMBER;
				this->Motor2Current = this->measure / ADC_MEASUREMENTS_NUMBER;
				this->measure = 0;
				
				this->turnOn();
				this->startConversion();
			}
		}
		break;
		
	}
}
