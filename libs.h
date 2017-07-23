/*
 * motors.h
 *biblioteka do oblugi silnikow
 * Created: 2016-02-28 23:03:55
 *  Author: Dominik
 */ 


#ifndef LIBS_H_
#define LIBS_H_

#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"
//-----------STALE--------------
#define STOP	1
#define LUZ		0
#define PRAWO	2
#define LEWO	3
//------------REJESTRY---------
#define L_PWM	TCD0.CCD //zwraca wypelnienie PWM silniku L
#define R_ENKODER TCF0.CNT //zwraca stan enkodera silnik L
#define R_PWM	TCD0.CCA //zwraca wypelnienie PWM silniku R
#define L_ENKODER TCC0.CNT //zwraca stan enkodera silnik R		


#define t_int 5.12 //czas miedzy dwoma przerwaniami dla PID i wyliczania pozycji.
//------------FUNKCJE-----------
uint8_t ReadCalibrationByte( uint8_t index );//kalibracja adc przez DMA
void setADC(void);//ustawia ADC
void setMotorL(void);//ustawia piny silnika L
void motorL(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
void setMotorR(void);//ustawia piny silnika R
void motorR(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
uint16_t adcPomiar_LD(void);
uint16_t adcPomiar_RD(void);//pomiar odleglosci
uint16_t adcPomiar_LF(void);
uint16_t adcPomiar_RF(void);
void OscXtal(void);
void setall(void);//ustawia wszystko
uint16_t adc_result_LD;
uint16_t adc_result_RD;
uint16_t adc_result_LF;
uint16_t adc_result_RF;

int old_enk_l; //Stara wartosc lewego enkodera (Potrzebne do wyznaczania pozycji)
int old_enk_r;
void ledYellow(void);
void ledGreen(void);
void setbat(void);
void Osc32MHz(void);
void OscPLL(uint8_t pllfactor);
uint16_t debancer (uint16_t p1,uint16_t p2,uint16_t p3);

#endif /* INCFILE1_H_ */