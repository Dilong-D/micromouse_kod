/*
 * pomiar.h
 *
 * Created: 18.10.2016 18:43:11
 *  Author: LPA1
 */ 


#ifndef POMIAR_H_
#define POMIAR_H_

#define INVERS 5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"




 
 
 short turn2(short direction);
 int16_t mouse_dir_x(short dir);
 int16_t mouse_dir_y(short dir);
 int16_t mierz (void);
 
 
 
 
 #endif