#include "real_alg.h"

//Licznik uzywany do ustalenia aktualnego etapu kroku
uint8_t nst = 2;


/**
* Funkcja wywolywane przez main do wykonania nastepnej czynnosci
*/
int action(void){
	if(nst == 2){
		com = getNextAction();
		nst = 0;
		return 0;
		} else {
		kalibruj();
		kal_count++;
		return move_real();
	}
}

/**
Krok do przodu w rzeczywistosci
*/
void step_forwards_real(void){
	switch(mouse_dir_real){
		case RIGHT:
		labposx_real++;
		break;
		case LEFT:
		labposx_real--;
		break;
		case UP:
		labposy_real++;
		break;
		case DOWN:
		labposy_real--;
		break;
	}
	if(mouse_dir_real == UP || mouse_dir_real == DOWN){
		//LcdClear();
		//LcdDec(par.posy);
		//Lcd(" ");
		//
		//Dec(OFFSET_Y + MM_NA_KRATKE*labposy_real);
		forward((float) OFFSET_Y + MM_NA_KRATKE*labposy_real, 1);
	}
	else
	forward((float) OFFSET_X + MM_NA_KRATKE*labposx_real, 0);
}

void turn_real(uint8_t direction){
	float rot;
	if(direction == RIGHT){
		switch(mouse_dir_real){
			case UP:
			mouse_dir_real = RIGHT;
			if(par.dir < PI){
				rot = -(-PI/2 - par.dir);
				rotateAngle(-(-PI/2 - par.dir));
			}
			else{
				rot = -(-PI/2 + (2 * PI - par.dir));
				rotateAngle(-(-PI/2 + (2 * PI - par.dir)));
			}
			
			break;
			case DOWN:
			mouse_dir_real = LEFT;
			rot = -(-PI/2 + (PI - par.dir));
			rotateAngle(-(-PI/2 + (PI - par.dir)));
			break;
			case RIGHT:
			mouse_dir_real = DOWN;
			rot = -(-PI/2 + (3*PI/2 - par.dir));
			rotateAngle(-(-PI/2 + (3*PI/2 - par.dir)));
			break;
			case LEFT:
			mouse_dir_real = UP;
			rot = -(-PI/2 + (PI/2 - par.dir));
			rotateAngle(-(-PI/2 + (PI/2 - par.dir)));
			break;
		}
		//LcdClear();
		//Lcd("Right ");
		//LcdDec(rot*360/(2*PI));
	}
	else if(direction == LEFT){
		switch(mouse_dir_real){
			case UP:
			mouse_dir_real = LEFT;
			if(par.dir < PI){
				rotateAngle(-(PI/2 - par.dir));
				rot = -(PI/2 - par.dir);
			}
			else{
				rotateAngle(-(PI/2 + (2 * PI - par.dir)));
				rot = (-(PI/2 + (2 * PI - par.dir)));
			}
			
			break;
			case DOWN:
			mouse_dir_real = RIGHT;
			rotateAngle(-(PI/2 + (PI - par.dir)));
			rot = (-(PI/2 + (PI - par.dir)));
			break;
			case RIGHT:
			mouse_dir_real = UP;
			rot = (-(PI/2 + (3*PI/2 - par.dir)));
			rotateAngle(-(PI/2 + (3*PI/2 - par.dir)));
			break;
			case LEFT:
			mouse_dir_real = DOWN;
			rot = (-(PI/2 + (PI/2 - par.dir)));
			rotateAngle(-(PI/2 + (PI/2 - par.dir)));
		}
		//LcdClear();
		//Lcd("LEFT ");
		//LcdDec(-rot*360/(2*PI));
		
	}
	if(par.dir >= 2*PI)
	par.dir -= 2*PI;
	
	if(par.dir < 0)
	par.dir += 2*PI;
}

uint8_t move_real(void){
	switch (com){
		case 5:
		if(nst == 0){
			turn_real(LEFT);
			turn_real(LEFT);
		}
		else
		step_forwards_real();
		break;
		case 6:
		if(nst == 0)
		turn_real(LEFT);
		else
		step_forwards_real();
		break;
		case 7:
		if(nst == 0)
		turn_real(RIGHT);
		else
		step_forwards_real();
		break;
		case 8:
		step_forwards_real();
		nst = 1;
		break;
		default:
		return 1;
		
	}
	if(mierz() == -1){
		return 2;
	}
	nst++;
	return 0;
}