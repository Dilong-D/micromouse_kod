/*
 * CFile1.c
 *
 * Created: 17.10.2016 20:42:48
 *  Author: LPA1
 */
#ifndef __MOVES_H_
#define __MOVES_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"


float des_vl;
float des_vr;
short kalibracja; // czy kalibracja jest potrzebna 0 - nie jest potrzebna,  1 kalibracja potrzebna


float abs2(float x);
void forward(float dest, int dir);
void rotateAngle(float angle);
void kalibruj(void);
void kalibruj_poczatek(void);
void kalibruj_ruchy(void);
uint8_t kal_count;

#endif