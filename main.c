/*
 * micromouse.c
 *
 * Created: 2016.10.15 02:30
 * Author : Dominik Markowski, Mateusz Wasala, Jan Zyczkowski, Piotr Papiez, Artur Hadasz
 */ 

#define  F_CPU    32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"
#include "libs.h"
#include "gyro.h"
#include "pid.h"
#include "pos_dir_enc.h"
#include "moves.h"
#include "algorithms.h"
#include "pomiar.h"
#include "real_alg.h"

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

uint8_t set = 0;

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
	     //ledYellow();   // wy³¹czenie diody LED 0
		 
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){
	ledYellow();        // wy³¹czenie diody LED 1
	
	
}

ISR(ACA_AC0_vect){
	PORTF_OUTTGL=PIN7_bm;

}

ISR(TCD1_OVF_vect){
	ledGreen();
	//wheel(des_vl, des_vr);
	//get_params_enc();
	//ledYellow();
}
//ISR(TCC1_OVF_vect){
	//
//}

ISR(OSC_OSCF_vect) {									// przerwanie w razie awarii oscylatora
	OSC.XOSCFAIL	|=	OSC_XOSCFDIF_bm;				// kasowanie flagi przerwania
	LcdClear();
	Lcd("Awaria!");
	_delay_ms(1000);
}



int main(void) {

	//int i;
	//int c2;
	setall();
	float ang;
	
		Lcd("Hello");
	_delay_ms(100);
	
// 	float max1 = 0;
// 	float min1 = 0;
	
 	//par.dir = 0;
 	//par.posx = 0;
 	//par.posy = 0;
 	//pid_init(8.33, 2.5760, 0, 8.57, 2.2988,0);
	 //rotateAngle(2);
	//des_vl = 2; des_vr = -20;
	
	
	runR(90,PRAWO);
	//runL(90,LEWO);
	while(1){	
		LcdClear();
	
		
		//ang = getAngleRadians();
		//
		//if( ang < 0)
		//{
			//Lcd("M");
			//LcdDec(abs2(ang)*10000);
			//if( ang < min1)
				//min1 = ang;
			//
			//
		//}
		//else {
			//Lcd("P");
			//LcdDec(abs2(ang)*10000);
			//if( ang > max1)
				//max1 = ang;
		//}
		//
		//Lcd2;
		//Lcd("MAX: ");
		//LcdDec((abs2(max1)*10000));
		//Lcd("MIN: ");
		//LcdDec((abs2(min1)*10000));
		//LcdDec(abs2(par.gyr));
		_delay_ms(100);
		//ledYellow(); 

	}
		
	
}