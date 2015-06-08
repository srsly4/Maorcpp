/*
 * MotorController.h
 *
 * Created: 2013-12-29 01:27:39
 *  Author: Sirius
 */ 
#include "ADCController.h"
#include <avr/io.h>

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

// Mostek M1
#define MLA 	           6
#define MLA_DDR 	       DDRC
#define MLA_PORT 	       PORTC

#define MLB 	           5
#define MLB_DDR 	       DDRC
#define MLB_PORT 	       PORTC

#define ML_PWM 	           4
#define ML_PWM_DDR 	       DDRD
#define ML_PWM_PORT 	   PORTD

#define ML_SENS 	       4
#define ML_SENS_DDR 	   DDRA
#define ML_SENS_PORT 	   PORTA


// Mostek M2
#define MRA 	           2
#define MRA_DDR 	       DDRC
#define MRA_PORT 	       PORTC

#define MRB 	           4
#define MRB_DDR 	       DDRC
#define MRB_PORT 	       PORTC

#define MR_PWM 	           5
#define MR_PWM_DDR 	       DDRD
#define MR_PWM_PORT 	   PORTD

#define MR_SENS 	       5
#define MR_SENS_DDR 	   DDRA
#define MR_SENS_PORT 	   PORTA

#define PWM_MAX  1023
#define MAX_PWM_SPEED  1023

#define MOTOR_MAX_VOLTAGE    700 

class MotorController
{
	public:
		MotorController(ADCController *adcController);
		void SetSpeed(signed char leftMotorSpeed, signed char rightMotorSpeed); //legacy function
		void SetLeftSpeed(signed char speed);
		void SetRightSpeed(signed char speed);
		
		signed char LeftEngine, RightEngine;
		
		
	private:
		ADCController *adc;
		unsigned long M1_last_speed, M2_last_speed;
};

#endif /* MOTORCONTROLLER_H_ */