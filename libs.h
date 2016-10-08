/*
 * motors.h
 *biblioteka do oblugi silnikow
 * Created: 2016-02-28 23:03:55
 *  Author: Dominik
 */ 


#ifndef LIBS_H_
#define LIBS_H_

#define F_CPU 2000000UL
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
#define L_ENKODER TCF0.CNT //zwraca stan enkodera silnik L
#define R_PWM	TCD0.CCA //zwraca wypelnienie PWM silniku R
#define R_ENKODER TCC0.CNT //zwraca stan enkodera silnik R		
//------------FUNKCJE-----------
uint8_t ReadCalibrationByte( uint8_t index );//kalibracja adc przez DMA
void setADC();//ustawia ADC
void setMotorL();//ustawia piny silnika L
void runL(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
void setMotorR();//ustawia piny silnika R
void runR(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
uint16_t adcPomiar_LD();
uint16_t adcPomiar_RD();//pomiar odleglosci
uint16_t adcPomiar_LF();
uint16_t adcPomiar_RF();
void setall();//ustawia wszystko
uint16_t adc_result_LD;
uint16_t adc_result_RD;
uint16_t adc_result_LF;
uint16_t adc_result_RF;
void setbat();
void ledYellow ();
void ledGreen();	
#endif /* INCFILE1_H_ */