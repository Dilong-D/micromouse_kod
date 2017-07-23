#include "pos_dir_enc.h"



struct par_enc get_params_enc(int16_t enkl,int16_t enkr,struct par_enc par){
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
	float w_gyr; //= getAngleRadians();
	
	
	
	//Wylicz predkosc lewego kola
	wayl=(float)((enkl* WHEEL_RADIUS_L * 2.0 * PI))/(CNTS_PER_REV * PRZEKLADNIA);
	wayr=(float)((enkr* WHEEL_RADIUS_R * 2.0 * PI))/(CNTS_PER_REV * PRZEKLADNIA);
	
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
		dx = wayr * sin(par.dir);
		dy = wayr * cos(par.dir);
	}
	
	//dx = dy;
	//Odpowiednio zmien stare wartosci
	par.dir = new_dir;
	par.posx += dx;
	par.posy =par.posy+ dy;
	w_gyr = getAngleRadians();
	
	if(w_gyr <= 0.02 && w_gyr >= -0.02){
		par.gyr  += 0;
	}
	else{
		par.gyr  += w_gyr; //razy czas? chyba 5us?
	}

	
	return par;
}