/*
 * ledcontroller.cpp
 *
 * Created: 2013-12-28 23:34:40
 *  Author: Sirius
 */ 
#include <avr/io.h>
#include "ledcontroller.h"

void LedController::TurnOn(unsigned char led_number)
{
	switch (led_number)
	{
		case LED_FL:
			LED_LD_FL_PORT|=(1<<LED_LD_FL);
		break;
		
		case LED_FR:
			LED_LD_FR_PORT |= (1 << LED_LD_FR);
		break;
		
		case LED_BL:
			LED_LD_BL_PORT |= (1 << LED_LD_BL);
		break;
		
		case LED_BR:
			LED_LD_BR_PORT |= (1 << LED_LD_BR);
		break;
		
		case LED_ALL:
			this->TurnOn(LED_FL);
			this->TurnOn(LED_FR);
			this->TurnOn(LED_BL);
			this->TurnOn(LED_BR);
		break;
	}
}

void LedController::TurnOff(unsigned char led_number)
{
	switch (led_number)
	{
		case LED_FL:
			LED_LD_FL_PORT &=~ (1<<LED_LD_FL);
		break;
		
		case LED_FR:
			LED_LD_FR_PORT &=~ (1 << LED_LD_FR);
		break;
		
		case LED_BL:
			LED_LD_BL_PORT &=~ (1 << LED_LD_BL);
		break;
		
		case LED_BR:
			LED_LD_BR_PORT &=~ (1 << LED_LD_BR);
		break;
		
		case LED_ALL:
			this->TurnOff(LED_FL);
			this->TurnOff(LED_FR);
			this->TurnOff(LED_BL);
			this->TurnOff(LED_BR);
		break;
	}
}

void LedController::Switch(unsigned char led_number)
{
	switch (led_number)
	{
		case LED_FL:
			LED_LD_FL_PORT ^= (1<<LED_LD_FL);
		break;
		
		case LED_FR:
			LED_LD_FR_PORT ^= (1 << LED_LD_FR);
		break;
		
		case LED_BL:
			LED_LD_BL_PORT ^= (1 << LED_LD_BL);
		break;
		
		case LED_BR:
			LED_LD_BR_PORT ^= (1 << LED_LD_BR);
		break;
		
		case LED_ALL:
			this->Switch(LED_FL);
			this->Switch(LED_FR);
			this->Switch(LED_BL);
			this->Switch(LED_BR);
		break;
	}
}

LedController::LedController()
{
	LED_LD_FL_DDR|=(1<<LED_LD_FL);
	LED_LD_FR_DDR|=(1<<LED_LD_FR);
	LED_LD_BL_DDR|=(1<<LED_LD_BL);
	LED_LD_BR_DDR|=(1<<LED_LD_BR);
}