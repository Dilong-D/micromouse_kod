#ifndef POS_DIR_ENC
#define POS_DIR_ENC

#include "math.h"
#include "libs.h"
#include "gyro.h"
//Promien kola mm
#define WHEEL_RADIUS_R 16.2
#define WHEEL_RADIUS_L 16.2
//Dlugosc osi (odleglosc miedzy srodkami pol) mm
#define SHAFT_LENGTH 90
#define PI 3.1415926
//Liczba impulsow na pelny obrot
#define CNTS_PER_REV 12
#define PRZEKLADNIA 10 //Przekladnia na silniku

float get_way_left();
float get_way_right();

void get_params_enc();



struct par_enc{
	//W radianach - kat osi wzgledem prostej x
	float dir;
	float posx;
	float posy;
	float gyr_dir;
};

struct par_enc par;

#endif
