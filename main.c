/*
 * micromouse.c
 *
 * Created: 2016.10.15 02:30
 * Author : Dominik Markowski, Mateusz Wasala, Jan Zyczkowski, Piotr Papiez, Artur Hadasz
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
#include "pid.h"
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
E4,E5,E6,E7	-	dioda IR
B0-B4	-	fototranzystory
C3-C7-	lcd
*/



// Interrupt Service Routine for handling the ADC conversion complete interrupt
ISR(ADCB_CH0_vect) {
	adc_result_LD = ADCB.CH0RES;
}
ISR(ADCB_CH1_vect) {
	adc_result_LF = ADCB.CH1RES;
}
ISR(ADCB_CH2_vect) {
	adc_result_RF = ADCB.CH2RES;
}
ISR(ADCB_CH3_vect) {
	adc_result_RD = ADCB.CH3RES;
}
//Przerwanie przycisk dol
ISR(PORTF_INT0_vect){
	ledGreen();        // wy³¹czenie diody LED 0
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){
	ledYellow();        // wy³¹czenie diody LED 1
}
ISR(ACA_AC0_vect){
	PORTF_OUTSET=PIN7_bm;

}
ISR(TCC1_OVF_vect){
	ledGreen();
}






int main(void) {
	setall();
	//runL(85,LEWO);
	//runR(85,LEWO);
	pid_init(0.006336,0.137,0,0.005912,0.1182,0);
	while(1){
		adcPomiar_LD();
		adcPomiar_RD();
		adcPomiar_LF();
		adcPomiar_RF();
		LcdClear();
		Lcd1;
		Lcd("LF");
		LcdDec(adc_result_RF);
		Lcd2;
		Lcd("LD");
		LcdDec(adc_result_LF);
		l_wheel(-10); // ujemne do przodu
		r_wheel(10); // dodatnie do przodu
		
		_delay_ms(20);
		
		};
}
	
	
		
		

			
		
	

