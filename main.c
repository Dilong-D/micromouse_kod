/*
 * micromouse_test.c
 *
 * Created: 2016-03-13 22:03:23
 * Author : Dominik Markowski
 */ 

#define  F_CPU    2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"
#include "libs.h"
#include "gyro.h"
#include "L3GD20H.h"
/*
Opis portów
port	funkcja		
D0	-	PWM	MOTOR R
D1	-	INPUT MOTOR 2 R
D2	-	INPUT MOTOR 1 R
D3	-	PWM	MOTOR L
D4	-	INPUT 2 MOTOR L
D5	-	INPUT 1	MOTOR L
C0	-	enkoder MOTOR L
C1	-	enkoder	MOTOR L
F0	-	enkoder MOTOR R
F1	-	enkoder	MOTOR R
E3,E4,E6,E7	-	dioda IR
B0-B4	-	fototranzystory
C3-C7-	lcd
E5	-	przycisk
*/



// Interrupt Service Routine for handling the ADC conversion complete interrupt
ISR(ADCB_CH0_vect) {
	adc_result_LD = ADCB.CH0RES;
}
ISR(ADCB_CH1_vect) {
	adc_result_RD = ADCB.CH1RES;
}
ISR(ADCB_CH2_vect) {
	adc_result_LF = ADCB.CH2RES;
}
ISR(ADCB_CH3_vect) {
	adc_result_RF = ADCB.CH3RES;
}
//Przerwanie przycisk dol
ISR(PORTF_INT0_vect){
	ledGreen();
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){
	ledRed();
}

int main(void) {
	setall();
	ledRed();
	runR(20,0);
	runL(20,0);
	
	
/*	while(1)
	{
		LcdClear();
		LcdDec(L_ENKODER);
		Lcd2;
		LcdDec(R_ENKODER);
		_delay_ms(200);
		
	};
	*/
TWI_MasterInit();
enableDefault();


struct gyro_xyz dane;
//uint8_t i;

while(1) {
	
	//who = readByte(REG_OUT_X_L);
	//	break;
	dane = readByteMulti();
	
	LcdClear();
	Lcd("x ");
	LcdDec(dane.x);
	Lcd2;
	Lcd("y ");
	LcdDec(dane.y);
	_delay_ms(500);
};
}
	
	
		
		

			
		
	

