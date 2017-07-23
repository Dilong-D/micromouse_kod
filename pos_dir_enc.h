#ifndef POS_DIR_ENC
#define POS_DIR_ENC

#include "math.h"
#include "libs.h"
#include "gyro.h"
//Promien kola mm
#define WHEEL_RADIUS_R 15.6799
#define WHEEL_RADIUS_L 15.6799
//Dlugosc osi (odleglosc miedzy srodkami pol) mm
#define SHAFT_LENGTH 90.0
#define PI 3.1415926
//Liczba impulsow na pelny obrot
#define CNTS_PER_REV 12.0
#define PRZEKLADNIA 50.0 //Przekladnia na silniku

float get_way_left(void);
float get_way_right(void);




struct par_enc{
	//W radianach - kat osi wzgledem prostej x
	float dir;
	float posx;
	float posy;
//	float gyr_dir;
	float gyr;
};

struct par_enc par;

struct par_enc get_params_enc(int16_t enkl,int16_t enkr,struct par_enc par);


#endif
