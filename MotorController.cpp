/*
 * MotorController.cpp
 *
 * Created: 2013-12-29 01:29:02
 *  Author: Sirius
 */ 
#define F_CPU 16000000UL

#include "MotorController.h"
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

MotorController::MotorController( ADCController *adcController )
{
	adc = adcController;
	
	MLA_DDR|=(1<<MLA);
	MLB_DDR|=(1<<MLB);
	MRA_DDR|=(1<<MRA);
	MRB_DDR|=(1<<MRB);
	
	MLA_PORT&=~(1<<MLA);
	MLB_PORT&=~(1<<MLB);
	MRA_PORT&=~(1<<MRA);
	MRA_PORT&=~(1<<MRB);
	
	ML_PWM_DDR|=(1<<ML_PWM);
	MR_PWM_DDR|=(1<<MR_PWM);
	
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM10)|(1<<WGM11);
	TCCR1B|=(1<<WGM12)|(1<<CS11)|(1<<CS10);	// Tryb 10-bit Fast PWM inverting mode,
	//TCCR1B|=(1<<WGM12)|(1<<CS12);
	// clk/64, co daje okres ok 4ms
	// Mostki sterujace silnikami
	TCCR1A &= ~((1<<COM1A1) | (1<<COM1B1)); // wylaczenie PWMa
}



void MotorController::SetLeftSpeed( signed char speed )
{
	if (speed == 0)
	{
		TCCR1A &= ~(1<<COM1B1);
		MLA_PORT &= ~(1<<MLA);
		MLB_PORT &= ~(1<<MLB);
	}
	if (speed > 0)
	{
		MLA_PORT |= (1<<MLA);
		MLB_PORT &= ~(1<<MLB);
		TCCR1A |= (1<<COM1B1);
	}
	if (speed < 0)
	{
		MLA_PORT &= ~(1<<MLA);
		MLB_PORT |= (1<<MLB);
		TCCR1A |= (1<<COM1B1);
	}
	
	unsigned int tmpSpeed = (unsigned int)(((float)abs(speed) / 100.0) * 1023.0);
	if(tmpSpeed > PWM_MAX)
		OCR1B = (unsigned int)PWM_MAX;
	else
		OCR1B = (unsigned int)tmpSpeed;
}

void MotorController::SetRightSpeed( signed char speed )
{
	if (speed == 0)
	{
		TCCR1A &= ~(1<<COM1A1);
		MRA_PORT &= ~(1<<MRA);
		MRB_PORT &= ~(1<<MRB);
	}
	if (speed > 0)
	{
		MRA_PORT |= (1<<MRA);
		MRB_PORT &= ~(1<<MRB);
		TCCR1A |= (1<<COM1A1);
	}
	if (speed < 0)
	{
		MRA_PORT &= ~(1<<MRA);
		MRB_PORT |= (1<<MRB);
		TCCR1A |= (1<<COM1A1);
	}
	
	unsigned int tmpSpeed = (unsigned int)(((float)abs(speed) / 100.0) * 1023.0);
	if(tmpSpeed > PWM_MAX)
		OCR1A = (unsigned int)PWM_MAX;
	else
		OCR1A = (unsigned int)tmpSpeed;
}

void MotorController::SetSpeed( signed char leftMotorSpeed, signed char rightMotorSpeed )
{
	this->SetLeftSpeed(leftMotorSpeed);
	this->SetRightSpeed(rightMotorSpeed);
}

