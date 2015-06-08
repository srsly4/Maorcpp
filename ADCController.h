/*
 * ADCController.h
 *
 * Created: 2013-12-29 00:32:29
 *  Author: Sirius
 */ 


#ifndef ADCCONTROLLER_H_
#define ADCCONTROLLER_H_

// Pomiar napiecia baterii
#define BAT_SENS	       3
#define BAT_SENS_DDR 	   DDRA
#define BAT_SENS_PORT 	   PORTA
#define BAT_SENS_PIN 	   PINA

#define ADC_MEASUREMENTS_NUMBER 38
#define ADC_MEASUREMENTS_OFFSET 10
#define ADC_VOL_MUL 0.0025F

class ADCController {
	private:
		void waitForConversionComplete(void);
		
		unsigned int measure;
		unsigned char adcChannel;
		unsigned char measurement_count;
	public:
		ADCController();
		
		void startConversion(void);
		void stopConversion(void);
		
		void turnOff(void);
		void turnOn(void);
		void selectChannel(unsigned char channel);
		unsigned int getOffsetError(void);
		
		void event_conversion_completed();
		
		//pomiary wykonane przez ADC
		volatile unsigned int Motor1Current;
		volatile unsigned int Motor2Current;
		volatile unsigned int BatteryADC;
		
		volatile unsigned int ADC0Measurement;
		volatile unsigned int ADC1Measurement;
		volatile unsigned int ADC2Measurement;
	};


#endif /* ADCCONTROLLER_H_ */