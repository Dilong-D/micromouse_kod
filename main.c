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
#include "algorithms.h"
#include "pomiar.h"
#include "real_alg.h"
#include "NewControl.h"
#include "bluetooth.h"

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

float regV_P =6;
float regV_I =2;//4.5;
float regV_D =0.48;//1.1;
//int16_t aktualna_V_8=0;
//int16_t target_S=0;

float regT_P = 6;
float regT_I = 2;
float regT_D = 0.48;
//int16_t aktualna_T_8=0;
int indTcc1=0;
int pred_ch[4];
int roznica_ch[4];
int roznica=0;
int pred=0;
int k=2000;
int pomiar=0;
int wyslij=0;
int tabsend[1000];
int pomiary[4][5];
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
	//reg_newPWML=0;
	//reg_newPWMP=0;
	aktualna_T_8=0;
	aktualna_V_8=0;
	pomiar=0;
	_delay_ms(200);
	regV_P =regV_P+1;
	regT_P=regT_P+1;
	//motorL(0,LEWO);
	
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){	
	//reg_newPWML=20;
	//reg_newPWMP=20;
	k=0;
	_delay_ms(1000);
	left(PI);
	////_delay_ms(5000);
	pomiar=1;
	_delay_ms(5000);
}

ISR(ACA_AC0_vect){
	PORTF_OUTTGL=PIN7_bm;

}
int32_t enkl=0;
int32_t enkr=0;
int16_t roznica_czujnikow;
int flaga_czuj=0;
int refR=0;
int refL=0;
ISR(TCD1_OVF_vect){
	pomiarr=adcPomiar_RD();
	pomiarl=adcPomiar_LD();
	pomiarfr=adcPomiar_RF();
	pomiarfl=adcPomiar_LF();
	
	
	int i,j;
	for(i=4;i>0;i--)
	   for(j=0;j<4;j++)
	       pomiary[j][i]=pomiary[j][i-1];
	
	pomiary[0][0]  =pomiarr;
	pomiary[1][0]  =pomiarl;
	pomiary[2][0]  =pomiarfr;
	pomiary[3][0]  =pomiarfl;
	
	stan_T = JAZDA;
	
	//if(((pomiary[0][0]+pomiary[0][1]+pomiary[0][2])>750)&&(pomiary[1][0]+pomiary[1][1]+pomiary[1][2]>750))
	if(((pomiarr)>100)&&((pomiarl)>100))
		{
			roznica_czujnikow=pomiarl-pomiarr;
			//PORTF_OUTSET=PIN5_bm;
			//PORTF_OUTSET=PIN6_bm;
			//flaga_czuj=1;
			refR=pomiarr;
			refL=pomiarl;
		}
	else if ((pomiarr>100)&&(pomiarl<100))
	    {
			roznica_czujnikow=refR-pomiarr;
			
			//roznica_czujnikow=pomiary[0][0]+pomiary[0][1]+pomiary[0][2]+pomiary[0][3]+pomiary[0][4]-pomiarr;
			//PORTF_OUTSET=PIN5_bm;
			//PORTF_OUTCLR=PIN6_bm;
			//flaga_czuj=0;
		}
	//else if((pomiary[1][0]+pomiary[1][1]+pomiary[1][2]>750))
	else if ((pomiarr<100)&&(pomiarl>100))	
	{
		roznica_czujnikow=-refL+pomiarl;
        //roznica_czujnikow=pomiarl-pomiary[1][0]-pomiary[1][1]-pomiary[1][2]-pomiary[1][3]-pomiary[1][4];
		//PORTF_OUTCLR=PIN5_bm;
		//PORTF_OUTSET=PIN6_bm;
	}
	else 
	{ 
		//PORTF_OUTCLR=PIN5_bm;
		//PORTF_OUTCLR=PIN6_bm;
		roznica_czujnikow=0;
	}
	if(flaga_czuj==1)
	{
		if(roznica_czujnikow>0)
		    {
			docelowa_T_8=-10;//roznica_czujnikow/parametrp; minus skreca w lewo
			target_T=lookup(roznica_czujnikow)*(aktualna_V_8/30)*537.6;
		    }
		else if (roznica_czujnikow<0)
		    {
			docelowa_T_8=10;//roznica_czujnikow/parametrp;
			target_T=lookup(roznica_czujnikow)*(aktualna_V_8/30)*537.6;
		    }
			else
			 {
				 docelowa_T_8=0;
				 target_T=0;
			 }
	}
	
}
		
	int enkl_send=0;
	int enkr_send=0;
ISR(TCC1_OVF_vect){
	//odczyt enkoderow
	enkl = L_ENKODER;
	enkr = R_ENKODER;
	L_ENKODER=0;
	R_ENKODER=0;
	enkl = - enkl;
	if(enkl > 30000)
		enkl = enkl -65536;
	if(enkr > 30000)
		enkr =enkr- 65536;
	//predkosc i roznica
	roznica=enkl-enkr;
	pred=enkl+enkr;
	//enkl_send=enkl_send+enkl;
	//enkr_send=enkr_send+enkr;
	//pid
	go(regV_P,regV_I,regV_D,pred, regT_P, regT_I, regT_D, roznica,enkl,enkr);
	
	
	////pozycja
	par=get_params_enc(enkl,enkr,par);
	//if(pomiar==1 && k<200)
	//{
		//k++;
		//send_char(par.posx);
		//send_char(par.posy);	
	 //}
	 //if(k>=200)
		//pomiar=0;
	
}

ISR(OSC_OSCF_vect) {									// przerwanie w razie awarii oscylatora
	OSC.XOSCFAIL	|=	OSC_XOSCFDIF_bm;				// kasowanie flagi przerwania
	LcdClear();
	Lcd("Awaria!");
	_delay_ms(1000);
}







int main(void) {
	setall();
	par.dir = 0;
	par.posx = 0;
	par.posy = 0;
	par.gyr=0;
	int d=0;
	enkl=0;
	enkr=0;
	flaga_czuj=1;
	forwardv2(6*180);
	int pomiarflsr;
	int pomiarfrsr;
	//flaga_czuj=1;
	//forwardv2(6*180);
	//_delay_ms(3500);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(2*180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(2*180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//right(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(2*180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//right(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(170);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(2*180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(180);
	//_delay_ms(1000);
	//flaga_czuj=0;
	//left(PI/2);
	//_delay_ms(1000);
	//flaga_czuj=1;
	//forwardv2(180);
	//_delay_ms(1000);
	
	while(1)
	{
		send_char((char)pomiarr%256);
		send_char((char)(pomiarr/256));
		
		send_char((char)pomiarl%256);
		send_char((char)(pomiarl/256));
		
		send_char((char)pomiarfr%256);
		send_char((char)(pomiarfr/256));
		
		send_char((char)pomiarfl%256);
		send_char((char)(pomiarfl/256));
		
		int asas=(int)(par.posy/10);
		send_char((char)asas);
		
		asas=(int)(par.posx/10+20);
		send_char((char)asas);	
		
		
		
		pomiarfrsr=(pomiary[2][0]+pomiary[2][1]+pomiary[2][2]+pomiary[2][3]+pomiary[2][4])/5;
		pomiarflsr=(pomiary[3][0]+pomiary[3][1]+pomiary[3][2]+pomiary[3][3]+pomiary[3][4])/5;
		if(((pomiarfrsr)<39)&&((pomiarflsr)<45))
		{
			PORTF_OUTSET=PIN5_bm;
			PORTF_OUTCLR=PIN6_bm;
			
			
		}
		else if(((pomiarfrsr)<194)&&((pomiarflsr)<219))
		{
			PORTF_OUTCLR=PIN5_bm;
			PORTF_OUTSET=PIN6_bm;
			
			
		}
		else if(((pomiarfrsr)>194)&&((pomiarflsr)>219))
		{
			PORTF_OUTSET=PIN5_bm;
			PORTF_OUTSET=PIN6_bm;
			
			
		}
		
		
		else 
		{
			PORTF_OUTCLR=PIN5_bm;
			PORTF_OUTCLR=PIN6_bm;
			
			
		}
		
		
		
		
		
		
		
		
	}
	
	
	
	
}

