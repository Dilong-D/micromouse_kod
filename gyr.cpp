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
#include "pos_dir_enc.h"
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

float set_vel_L = 0;
float set_vel_R = 0; 
uint8_t flag_set_vel = 0;
uint16_t count = 0;
uint8_t max_enc = 0;
uint16_t pom[51] = {0};

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
	ledGreen();        // wyłączenie diody LED 0
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){
	ledYellow();        // wyłączenie diody LED 1
}
ISR(ACA_AC0_vect){
	PORTF_OUTSET=PIN7_bm;

}
ISR(TCC1_OVF_vect){
	ledGreen();
	//if(flag_set_vel == 1){
		//l_wheel(set_vel_L); // ujemne do przodu
		//r_wheel(set_vel_R); // dodatnie do przodu
		//
	//}
}

ISR(TCD1_OVF_vect){
	
	ledGreen();
	//get_params_enc();
	//old_enk_l = L_ENKODER;
	//L_ENKODER = 0;
	//old_enk_r = R_ENKODER;
	//R_ENKODER = 0;
	//if(count > 200){
		//runR(0, PRAWO);
		//wheel(0, 0);
		/*LcdClear();
		Lcd1;
		LcdDec(old_enk_l);
		Lcd(" ");
		LcdDec(old_enk_r);*/
		//LcdClear();
		//Lcd("x: ");
		//count = 0;
		////R_ENKODER=0;
		////L_ENKODER=0;
		////return;
	//}
	//count++;
	//else if(flag_set_vel == 1){
	//	wheel(5, 5);
		//get_params_enc();
		//l_wheel(-1); // ujemne do przodu
		//r_wheel(5);
		//l_wheel(5); // dodatnie do przodu
		//uint16_t old_renk = R_ENKODER;
		//R_ENKODER=0;
		//uint16_t old_lenk = L_ENKODER;
		//L_ENKODER=0;
		//count++;
		//}
	
}
//ISR(OSC_OSCF_vect) {									// przerwanie w razie awarii oscylatora
	//OSC.XOSCFAIL	|=	OSC_XOSCFDIF_bm;				// kasowanie flagi przerwania
	//LcdClear();
	//Lcd("Awaria!");
//}




int main(void) {
	int i;
	int c2;
	//setall();
	
	LcdInit();
	LcdClear();
	Lcd("init!");
	_delay_ms(500);
	struct gyro_xyz dane;
	TWI_MasterInit();
	enableDefault();

	while(1){
	
	dane = readByteMulti();
	//if(dane.x < 0){
		//LcdClear();
		//Lcd("x:");
		//Lcd2;
		//LcdDec(-dane.x);
	//}else if(dane.x >= 0){
		//LcdClear();
		//Lcd("x:");
		//Lcd2;
		//LcdDec(dane.x);
	//}
	//_delay_ms(500);
	
	if(dane.y < 0){
		LcdClear();
		Lcd("minusy:");
		Lcd2;
		LcdDec(-dane.y*SENSITIVITY_2000DPS*DPS_TO_RADS);
		}else if(dane.y >= 0){
		LcdClear();
		Lcd("y:");
		Lcd2;
		LcdDec(dane.y*SENSITIVITY_2000DPS*DPS_TO_RADS);
	
	}_delay_ms(100);
	}
	
}