#ifndef _APP_H
#define _APP_H

#include "includes.h"

/* Main settings */
/* comment the below line for release */
//#define TESTING
//#define DEMO

#define PhNum1	 "PHONE_NUMBER1"
#define PhNum2	 "PHONE_NUMBER2"


struct  App {
	volatile char Fuel	:1;
	volatile char Meas	:1;
	volatile char Msg	:1;
};


/*********** EXTERNAL INTERRUPT PINS ***************/
#define INTDDR					DDRD
#define INTPORT					PORTD
#define INTPIN					PIND
#define INT0_PIN				PD2
#define INT1_PIN				PD3

#define EEPROM_DISPLAY				0

/******** MAIN CONFIGURATION ENDS ****************/

/* LCD CONFIGURATION */
#define LCD_DDR           	   		DDRC 
#define LCD_DATA_PORT          		PORTC
#define LCD_ENABLE_PIN        		PC3 
#define LCD_REGISTER_SELECT_PIN     PC2
#define DISPLAY_INIT				1
#define LCD_NSCRL					3
#define FLOAT_T0_STRING				1
#define DECIMAL_PRECISION			100

/******** LCD CONFIGURATION ENDS ****************/

/* MOTOR CONFIGURATION */

#define MOTOR_DDR				DDRC
#define MOTOR_PORT				PORTC
#define MOT_ANODE_PIN			PC2
#define MOT_CATHODE_PIN			PC3

/******** LCD CONFIGURATION ENDS ****************/


/* BUZZER: CONFIGURATION */
#define	BUZ_DDR						DDRC
#define	BUZ_PORT					PORTC
#define	BUZ_PIN						PC1
#define OFF_TIME					100
/******** BUZZER CONFIGURATION ENDS ****************/

/* LED: CONFIGURATION */

#define	LED_DDR			DDRC
#define	LED_PORT		PORTC
#define	LED_PIN			PC0
#define OFF_TIME		100
/******** LED CONFIGURATION ENDS ****************/

/* GSM: CONFIGURATION */

#define MSG_START					""
#define MSG_END						". Automated SMS By: Digital Fuel Guage."

/******** ADC CONFIGURATION ENDS ****************/
#define REF_VOL_256

#define GSM_GPS_DDR				DDRD
#define GSM_GPS_PORT			PORTD
#define GSM_GPS_PIN				PD7

#define GSMEn()					do {									\
									GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
									dlyms(500);							\
								} while(0)

#define GPSEn()					do {									\
									GSM_GPS_PORT |= _BV(GSM_GPS_PIN);	\
									dlyms(500);							\
								} while(0)
/******** GSM CONFIGURATION ENDS ****************/

/******** EEPROM CONFIGURATION ****************/
#define EEPROM_DISPLAY		0


/********* HX711 24-BIT ADC ****************/
#define	HX711_DDR			DDRD
#define HX711_CLK_PIN		PD5
#define HX711_DIN_PIN		PD6
#define HX711_PORT			PORTD
#define HX711_INPIN			PIND
/******************************************/

#endif
