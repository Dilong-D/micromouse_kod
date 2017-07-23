/*
 * NewControl.c
 *
 * Created: 2016-12-16 19:05:37
 *  Author: Pawel
 */ 
 #include "moves.h"
 #include "algorithms.h"
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/pgmspace.h>
 #include <stddef.h>
 #include "hd44780.h"
 #include "libs.h"
 #include "gyro.h"
 #include "pomiar.h"
 #include "pid.h"
 #include "pos_dir_enc.h"
 #include "NewControl.h"
 #define  F_CPU 32000000UL

 target_S = 0; // zadana droga	 
 stan = JAZDA;            // stan profilera; mo¿liwe wartoœci: JAZDA, HAMOWANIE, KONIEC
 docelowa_V_8 = 30;        // docelowa_V - docelowa prêdkoœæ, do której stopniowo d¹¿y profiler
 //max_V - maksymalna dozwolona prêdkoœæ
 int16_t next_V_8 = 5;                // prêdkoœæ w nastêpnym ruchu (po przejechaniu zadanej odleg³oœci)
             // aktualnie zadana prêdkoœæ (impulsy enkodera na przerwanie)
 int16_t przysp_8 = 1;                // zadane przyspieszenie 
 float kat = 0;

 sum = 0;
 reg_newPWML = 0;
 reg_newPWMP = 0;
 aktualna_V_8 = 0;
 deb = 0;
 regT_aktE = 0;
 float zad_T = 0;

 float regV_prevE = 0;
 float regT_prevE = 0;
 float enk_totalS = 0;
 float enk_totalT = 0;
 float enk_sumS = 0;
 float enk_sumT = 0;
 
 int lookup(int roznicaczujnikow)
 {	int i;
	 int tabe [2][17]={
		 {10   ,  8   ,   7 ,    5, 4  , 3  ,2  ,1 ,0,-1,-2 ,-3 ,-4 ,-5 ,-7  ,-8  ,   -10} ,
		 {-2000, -1000, -700, -400,-200,-100,-40,-5,5,40,100,200,400,700,1000,2000, 10000}
	 };
	 for(i=0;i<16;i++)
	 if (roznicaczujnikow<tabe[1][i])
	 return tabe[0][i];
	 
	 return 0;

 }

 void profiler(int32_t enkl,int32_t enkr)
 {
	 if(docelowa_V_8 >= 0){
		 if(stan == JAZDA)
		 {
			 if((aktualna_V_8 - next_V_8)*(aktualna_V_8 + next_V_8 )/(4*przysp_8) >=abs2(target_S))
			 {
				 stan = HAMOWANIE;
				 docelowa_V_8 = 5;
			 }
		 }
		 if(aktualna_V_8 < docelowa_V_8)
		 {
			 if(aktualna_V_8 < 5)
				aktualna_V_8 = 5;
			 else
				 aktualna_V_8 += przysp_8;
			 if(aktualna_V_8 > docelowa_V_8)
				 aktualna_V_8 = docelowa_V_8;
		 }
	 
		 if(aktualna_V_8 > docelowa_V_8)
		 {
			 aktualna_V_8 -= przysp_8;
			 if(aktualna_V_8 < docelowa_V_8)
				aktualna_V_8 = docelowa_V_8;
		 }
		 
		 if(target_S <= 0)
		 {
			 aktualna_V_8 = 0;
		 }
	}else{
		
		if(stan == JAZDA)
		{
			if((aktualna_V_8 - next_V_8)*(aktualna_V_8 + next_V_8 )/(4*przysp_8) >=abs2(target_S))
			{
				stan = HAMOWANIE;
				docelowa_V_8 = -5;
			}
		}
		 if(aktualna_V_8 > docelowa_V_8)
		 {
			 if(aktualna_V_8 > -5)
				aktualna_V_8 = -5;
			 else
				aktualna_V_8 -= przysp_8;
			 if(aktualna_V_8 < docelowa_V_8)
				aktualna_V_8 = docelowa_V_8;
		 }
		 
		 if(aktualna_V_8 < docelowa_V_8)
		 {
			 aktualna_V_8 += przysp_8;
			 if(aktualna_V_8 > docelowa_V_8)
			 aktualna_V_8 = docelowa_V_8;
		 }
		 
		 if(target_S > 0)
		 {
			 aktualna_V_8 = 0;
		 }
		 
	 }
	 
	 
	target_S -= enkl + enkr;    // zad_S jest typu ca³kowitego - nale¿y odpowiednio przekonwertowaæ zmienn¹ aktualna_V_8 przed dodaniem
 }
 

stan_T = JAZDA;        // stan profilera; mo¿liwe wartoœci: JAZDA, HAMOWANIE, KONIEC
docelowa_T_8 = 5;        // docelowa_T - docelowa prêdkoœæ, do której stopniowo d¹¿y profiler
// max_T - maksymalna dozwolona prêdkoœæ
aktualna_T_8 =0;            // aktualnie zadana prêdkoœæ
float przyspT_8 = 0.5;                // zadane przyspieszenie
target_T = 0;                // zadany ca³kowity k¹t do przejechania
flag_T=0;


void profiler_k(int32_t enkl,int32_t enkr)
{
	
	if(docelowa_T_8 >= 0){
		if(stan_T == JAZDA)
		{
			if((aktualna_T_8 - 5) *(5 + aktualna_T_8)/(2 * przyspT_8) >= abs2(target_T))
			{
				stan_T = HAMOWANIE;
				docelowa_T_8 = 5;
			}
		}
		if(aktualna_T_8 < docelowa_T_8)
		{
			aktualna_T_8 += przysp_8;
			if(aktualna_T_8 > docelowa_T_8)
			aktualna_T_8 = docelowa_T_8;
		}
	
		if(aktualna_T_8 > docelowa_T_8)
		{
			aktualna_T_8 -= przysp_8;
			if(aktualna_T_8 < docelowa_T_8)
			aktualna_T_8 = docelowa_T_8;
		}
	
		if(target_T <= 0)
		{
			aktualna_T_8 = 0;
		}
	}else{
		if(stan_T == JAZDA)
		{
			if((aktualna_T_8 - 5) *(5 + aktualna_T_8)/(2 * przyspT_8) >= abs2(target_T))
			{
				stan_T = HAMOWANIE;
				docelowa_T_8 = -5;
			}
		}
		if(aktualna_T_8 > docelowa_T_8)
		{
			aktualna_T_8 -= przysp_8;
			if(aktualna_T_8 < docelowa_T_8)
				aktualna_T_8 = docelowa_T_8;
		}
		
		if(aktualna_T_8 < docelowa_T_8)
		{
			aktualna_T_8 += przysp_8;
			if(aktualna_T_8 > docelowa_T_8)
				aktualna_T_8 = docelowa_T_8;
		}
		
		if(target_T >= 0)
		{
			aktualna_T_8 = 0;
		}
	}
	target_T -= (enkl - enkr);
	
}
 
 void reg(float regV_P,float regV_I,float regV_D, int16_t suma_akt, float regT_P,float regT_I,float regT_D, int16_t roznica_akt)
 {
	 
	regV_aktE = aktualna_V_8 - suma_akt;
	enk_sumS=enk_sumS+regV_aktE;
	
	regT_aktE = aktualna_T_8 - roznica_akt;
	enk_sumT= enk_sumT+regT_aktE;
	
	reg_newPWML = regV_P * (regV_aktE + regV_D * (regV_aktE - regV_prevE)+(1/regV_I)*enk_sumS)+ regT_P * (regT_aktE+ regT_D * (regT_aktE - regT_prevE)+(1/regT_I)*enk_sumT);
	reg_newPWMP = regV_P * (regV_aktE + regV_D * (regV_aktE - regV_prevE)+(1/regV_I)*enk_sumS)- regT_P * (regT_aktE+ regT_D * (regT_aktE - regT_prevE)+(1/regT_I)*enk_sumT);
	
	regV_prevE = regV_aktE;
	regT_prevE = regT_aktE;
 }
 
void go(float regV_P,float regV_I,float regV_D, int suma_akt, float regT_P,float regT_I,float regT_D,int roznica_akt, int32_t enkl, int32_t enkr)
{
	profiler_k(enkl,enkr);
	//aktualna_V_8=10;
	profiler(enkl,enkr);
	
	//aktualna_T_8=0;
	reg(regV_P,regV_I,regV_D, suma_akt, regT_P, regT_I, regT_D, roznica_akt);
	//reg_newPWML=20;
	//reg_newPWMP=20;
	if(reg_newPWML >= 0)
		motorL(reg_newPWML,PRAWO);
	else
		motorL(-reg_newPWML,LEWO);
	
	if(reg_newPWMP >= 0)
		motorR(reg_newPWMP,LEWO);
	else
		motorR(-reg_newPWMP,PRAWO);
}

void forwardv2(int16_t dist)
{
	target_S = dist / 0.0821;
	target_T = 0;
	
	docelowa_T_8 = 0;
	docelowa_V_8 = 15;
	regV_aktE = 0;
	enk_sumS = 0;
	regT_aktE = 0;
	enk_sumT = 0;
	regV_prevE = 0;
	regT_prevE = 0;
	
	stan = JAZDA;
	stan_T = JAZDA;
}

//void forwardv3(int16_t dist)

void backwardv2(int16_t dist)
{
	target_S = -dist / 0.0821;
	target_T = 0;
	
	docelowa_T_8 = 20;
	docelowa_V_8 = -50;
	regV_aktE = 0;
	enk_sumS = 0;
	regT_aktE = 0;
	enk_sumT = 0;
	regV_prevE = 0;
	regT_prevE = 0;
	
	stan = JAZDA;
	stan_T = JAZDA;
}

void left(float angle)
{
	target_T = -angle * 537.6;
	docelowa_T_8 = -20;
	docelowa_V_8 = 0;
	target_S = 0;
	
	regV_aktE = 0;
	enk_sumS = 0;
	regT_aktE = 0;
	enk_sumT = 0;
	regV_prevE = 0;
	regT_prevE = 0;
	
	stan_T = JAZDA;
	stan = JAZDA;
}

void right(float angle)
{
	target_T = angle * 537.6;
	docelowa_T_8 = 20;
	docelowa_V_8 = 0;
	target_S = 0;
	
	regV_aktE = 0;
	enk_sumS = 0;
	regT_aktE = 0;
	enk_sumT = 0;
	regV_prevE = 0;
	regT_prevE = 0;
	
	stan_T = JAZDA;
	stan = JAZDA;
}












	 