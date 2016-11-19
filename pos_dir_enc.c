#include "pos_dir_enc.h"




/**
* predkosc kola w mm/ms
*/
float get_way_left(void){
	if(old_enk_l > 30000)
		old_enk_l -=  65536;
	return -(old_enk_l * WHEEL_RADIUS_L * 2 * PI)/(CNTS_PER_REV * PRZEKLADNIA);
}

float get_way_right(void){
	if(old_enk_r > 30000)
		old_enk_r -=  65536;
	return (old_enk_r * WHEEL_RADIUS_R * 2 * PI)/(CNTS_PER_REV * PRZEKLADNIA);
}

void get_params_enc(void){
	float wayl;
	float wayr;
	//predkosc katowa
	float ddir;
	//Nowy kat
	float new_dir;
	//Chwilowy promien kola,  po ktorym jedzie robot (srodek jego osi)
	float R;
	//Przyrost posx
	float dx;
	//Przyrost posy
	float dy;
	//w_gyr = getAngleRadians();
	
	//Wylicz predkosc lewego kola
	wayl = get_way_left();
	wayr= get_way_right();
	
	//Wylicz nowy kat
	ddir = ((wayr - wayl)/SHAFT_LENGTH);
	new_dir = par.dir + ddir;
	
	if(ddir >= 0.0001 || ddir <= -0.0001 ){
		//Wylicz chwilowy promien
		R = (wayl/ddir) + (SHAFT_LENGTH/2.0);
	
		//Przemieszczenia w osiach x i y
		
		dy = R * (sin(new_dir) - sin(par.dir));
		//dx = dy;
		dx = R * (cos(new_dir) - cos(par.dir));
	}
	else{
		//Jedzie na wprost
		dx = -wayr * sin(par.dir);
		dy = wayr * cos(par.dir);
	}
	
	//dx = dy;
	//Odpowiednio zmien stare wartosci
	par.dir = new_dir;
	par.posx += dx;
	par.posy += dy;

	
	
}