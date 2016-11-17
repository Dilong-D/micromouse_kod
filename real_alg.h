
#ifndef REAL_ALG_H_
#define REAL_ALG_H_

#define MM_NA_KRATKE 180
#define OFFSET_X 90
#define OFFSET_Y 90

#include "moves.h"
#include "algorithms.h"
#include "libs.h"
#include "pomiar.h"
#include "pos_dir_enc.h"

int action();
void step_forwards_real();
void turn_real(uint8_t direction);
uint8_t move_real();


//Zadanie do wykonania
uint8_t com;

#endif