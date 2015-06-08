/*
 * ledcontroller.h
 *
 * Created: 2013-12-28 23:29:23
 *  Author: Sirius
 */ 

//Kolejnoœæ diód
#define LED_FL 1
#define LED_FR 2
#define LED_BL 3
#define LED_BR 4
#define LED_ALL 0

//Piny
// LED przod-lewy
#define LED_LD_FL 	       7
#define LED_LD_FL_DDR 	   DDRA
#define LED_LD_FL_PORT 	   PORTA
// LED przod-prawy
#define LED_LD_FR 	       3
#define LED_LD_FR_DDR 	   DDRC
#define LED_LD_FR_PORT 	   PORTC
// LED tyl-lewy
#define LED_LD_BL 	       6
#define LED_LD_BL_DDR 	   DDRA
#define LED_LD_BL_PORT 	   PORTA
// LED tyl-prawy
#define LED_LD_BR 	       1
#define LED_LD_BR_DDR 	   DDRC
#define LED_LD_BR_PORT 	   PORTC
#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_


class LedController {
	public:
		void TurnOn(unsigned char led_number);
		void TurnOff(unsigned char led_number);
		void Switch(unsigned char led_number);
		LedController();
	};

#endif /* LEDCONTROLLER_H_ */