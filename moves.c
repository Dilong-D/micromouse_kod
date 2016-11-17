/*
 * CFile1.c
 *
 * Created: 17.10.2016 20:42:13
 *  Author: LPA1
 */ 
 #include "moves.h"
 #include "algorithms.h"
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/pgmspace.h>
 #include <stddef.h>
 #include "hd44780.h"
 #include "libs.h"
 #include "gyro.h"
  #include "pomiar.h"
 #include "pid.h"
 #include "pos_dir_enc.h"
  #define  F_CPU 32000000UL
  
  
  short kali_obrot;

 float abs2(float x)
 {
	 if(x>=0)
	 return x;
	 else
	 return -x;
 }


void forward(float dest, int dir) //dir==0 x ; dir==1 y
{
	float start;
	float start2;
	float delta;
	float dist_made;
	float VmaxL = 2;
	float VmaxR = 2;
	
	
	
	if(dir==1)
	{
		start=par.posy;
		start2=par.posx;
		float dist=abs2(dest-start);
		
		while(1)
		{
		//	_delay_ms(100);
			
			delta=(par.posx-start2)*0.00;
			dist_made = abs2(par.posy-start);
			
			
		//	LcdClear();
		//	Lcd("y: ");
			//LcdDec(par.posy);
			
			if(abs2(dest-start) - abs2(dist_made) <= 0.03)
			{
				des_vl=0;
				des_vr=0;
				_delay_us(0.001);
				break;
			}
			
			else if(dist*0.2 > dist_made)
			{
				des_vl = (5*VmaxL*dist_made/dist+0.1)*(1-delta);
				des_vr = (5*VmaxR*dist_made/dist+0.1)*(1+delta);
				_delay_us(0.001);
				PORTF_OUTSET=PIN6_bm;
			}
			
			else if(dist_made >= dist*0.2 && dist_made <= dist*0.9)
			{
				des_vl = (VmaxL+0.1)*(1-delta);
				des_vr = (VmaxR+0.1*(1+delta));
				_delay_us(0.001);
				
				PORTF_OUTCLR=PIN6_bm;
			}
			
			else if(dist*0.9 < dist_made)
			{
				
				des_vl = (-10*VmaxL*dist_made/dist + 0.1 + 10*VmaxL)*(1-delta);
				des_vr = (-10*VmaxR*dist_made/dist + 0.1 + 10*VmaxR)*(1+delta);  //(0.9*dist-dist_made)*3.3 + 3.3        ((0.9*dist - dist_made)/dist + 1)*3.3
				_delay_us(0.001);
				PORTF_OUTSET=PIN6_bm;
			}

		}
	}
	else if(dir==0)
	{
		start=par.posx;
		start2=par.posy;
		float dist=abs2(dest-start);
		
		while(1)
		{
			delta=(par.posy-start2)*0.00;
			dist_made = abs2(par.posx-start);
			
			if(abs2(dest-start) - abs2(dist_made)  <= 0.03)
			{
				LcdClear();
				des_vl=0;
				des_vr=0;
				_delay_us(0.001);
				break;
			}
			
			else if(dist*0.2 > dist_made)
			{
				des_vl = (5*VmaxL*dist_made/dist+0.1)*(1-delta);
				des_vr = (5*VmaxR*dist_made/dist+0.1)*(1+delta);
				_delay_us(0.001);
				PORTF_OUTSET=PIN6_bm;
			}
			
			else if(dist_made >= dist*0.1 && dist_made <= dist*0.9)
			{
				des_vl = (VmaxL+0.1)*(1-delta);
				des_vr = (VmaxR+0.1*(1+delta));
				_delay_us(0.001);
				
				PORTF_OUTCLR=PIN6_bm;
			}
			
			else if(dist*0.9 < dist_made)
			{
				
				des_vl = (-10*VmaxL*dist_made/dist + 0.1 + 10*VmaxL)*(1-delta);
				des_vr = (-10*VmaxR*dist_made/dist + 0.1 + 10*VmaxR)*(1+delta);  //(0.9*dist-dist_made)*3.3 + 3.3        ((0.9*dist - dist_made)/dist + 1)*3.3
				_delay_us(0.001);
				PORTF_OUTSET=PIN6_bm;
			}

		}
	}
	
	//float angpop=0;
	//for(int k  = 0; k < 4; ++k)
	//{
		//if(abs2(par.dir-PI*k/2) <= PI/4)
		//{
			//angpop = PI*k/2-par.dir;
			//break;
			//
		//}
	//}
	//
	//LcdClear();
	//LcdDec((int)(abs2(1000*angpop)));
	//Lcd2;
	//LcdDec((int)(abs2(1000*par.dir)));
	//_delay_ms(1000);
	//if(abs2(angpop) > 0.06)
	//rotateAngle(angpop);
		//
	
	
}

void rotateAngle(float angle) //dir==0 lewo ; dir==1 prawo
{
	float start;
	float dist_made;
	float VmaxL = 1;
	float VmaxR = 1;
	float dir_Vl;
	float dir_Vr;
	//angle*=0.99;
	
	if(abs2(angle)<0.3)
	{
		VmaxL = 2;
		VmaxR = 2;
	}
	
	if(angle < 0)
	{
		dir_Vl = -1.0;
		dir_Vr = 1.0;
		
		}else if(angle > 0){
		
		dir_Vl = 1.0;
		dir_Vr = -1.0;
	}
	
	
	start = par.dir;
	//float dist = angle+start;
	
	while(1)
	{
		dist_made = par.dir-start;
		
		if(abs2(angle)-abs2(dist_made) <= 0.02)
		{
			LcdClear();
			Lcd("QQQ");
			des_vl=0;
			des_vr=0;
			_delay_ms(1000);
			break;
			
		}
		else if(abs2(angle)*0.2 > abs2(dist_made))
		{
			
			des_vl = dir_Vl*(5*VmaxL*abs2(dist_made)/abs2(angle) + 0.1);
			des_vr = dir_Vr*(5*VmaxR*abs2(dist_made)/abs2(angle) + 0.1);
			_delay_us(0.001);
			LcdClear();
			Lcd("A");
		//	LcdDec(abs2(par.dir-dist)*100);

		}
		
		else if(abs2(angle)*0.2 <= abs2(dist_made) && abs2(angle*0.8) >= abs2(dist_made))
		{
			
			des_vl = dir_Vl*VmaxL+0.1;
			des_vr = dir_Vr*VmaxR+0.1;
			_delay_us(0.001);
			LcdClear();
			Lcd("B");
		//	LcdDec(abs2(par.dir-dist)*100);
		}
		else if(abs2(angle*0.8) < abs2(dist_made))
		{
			des_vl = dir_Vl*(-5*VmaxL*abs2(dist_made)/abs2(angle) + 5*VmaxL + 0.1);
			des_vr = dir_Vr*(-5*VmaxR*abs2(dist_made)/abs2(angle) + 5*VmaxR + 0.1);  //(0.9*dist-dist_made)*3.3 + 3.3        ((0.9*dist - dist_made)/dist + 1)*3.3
			_delay_us(0.001);
			LcdClear();
			Lcd("C");
		//	LcdDec(abs2(par.dir-dist)*100);
		}
		
	}
	
	
	
	float angpop=0;
	for(int k  = 0; k < 4; ++k)
	{
		if(abs2(par.dir-PI*k/2) <= PI/4)
		{
			angpop = PI*k/2-par.dir;
			break;
			
		}
	}
	
	
	LcdClear();
	Lcd("fff");
	LcdDec((int)(abs2(1000*angpop)));
	Lcd2;
	LcdDec((int)(abs2(1000*par.dir)));
	_delay_ms(1000);
	if(abs2(angpop) > 0.06)
	rotateAngle(angpop);

}

void kalibruj_ruchy()
{
	des_vl=-0.5;
	des_vr=-0.5;
	
	_delay_ms(22220);
	
	des_vl=0;
	des_vr=0;
	
	switch (mouse_dir_real)
	{
		case UP:
		par.posy=labposy_real*180+45;
		forward(par.posy+43,1);
		
		break;
		
		case DOWN:
		
		par.posy=labposy_real*180+135;
		forward(par.posy-43,1);
		break;
		
		case RIGHT:
		par.posx =labposx_real*180+45;
		forward(par.posx+43,0);
		break;
		
		case LEFT:
		par.posx =labposx_real*180+135;
		forward(par.posx-43,0);
		break;
	}
	
	
	
	if (((labyrinth[labposx_real][labposy_real]&turn2(RIGHT))==0))
	{rotateAngle(-PI/2);
		mouse_dir_real=turn2(LEFT);
		kali_obrot=1;
	}
	
	else
	{rotateAngle(PI/2);
		mouse_dir_real=turn2(RIGHT);
		kali_obrot=2;
	}
	
	
	
	des_vl=-0.5;
	des_vr=-0.5;
	_delay_ms(22220);
	des_vl=0;
	des_vr=0;
	
	switch (mouse_dir_real)
		{
			case UP:
			par.posy=labposy_real*180+45;
			forward(par.posy+43,1);
			
			break;
			
			case DOWN:
			
			par.posy=labposy_real*180+135;
			forward(par.posy-43,1);
			break;
			
			case RIGHT:
			par.posx =labposx_real*180+45;
			forward(par.posx+43,0);
			break;
			
			case LEFT:
			par.posx =labposx_real*180+135;
			forward(par.posx-43,0);
			break;
		}
	
	kal_count=0;
	
	
	
	switch (mouse_dir_real)
	{
		case UP:
		par.dir=0;
		break;
		
		case DOWN:
		par.dir=PI;
		break;
		
		case RIGHT:
		par.dir=PI/2;
		break;
		
		case LEFT:
		par.dir=3*PI/2;
		break;
	}
	
	
}



void kalibruj_poczatek()
{
	short debRF=debancer(adcPomiar_RF(),adcPomiar_RF(),adcPomiar_RF());  // pomiar RF
	_delay_ms(1);
	short	 debLF=debancer(adcPomiar_LF(),adcPomiar_LF(),adcPomiar_LF());  // pomiar LF
	_delay_ms(1);
	
	if(((debRF==-1)||(debRF==-1))|| (debRF!=debLF))
	;//return (-1);
	
	else
	if(debRF==0)
	labyrinth[0][0]&=(~UP);
	
	_delay_ms(1000);
	
	des_vl=-0.5;
	des_vr=-0.5;
	_delay_ms(22001);
	des_vl=0;
	des_vr=0;
	
	_delay_ms(22001);
	par.posy=43;
	forward(par.posy+30,1);
	rotateAngle(PI/2);

	debRF=debancer(adcPomiar_RF(),adcPomiar_RF(),adcPomiar_RF());  // pomiar RF
	_delay_ms(1);
	debLF=debancer(adcPomiar_LF(),adcPomiar_LF(),adcPomiar_LF());  // pomiar LF
	_delay_ms(1);

	if(((debRF==-1)||(debRF==-1))|| (debRF!=debLF))
	;//return (-1);
	else if(debRF==0)
	labyrinth[0][0]&=(~RIGHT);


	discovered[0][0]=1;
	des_vl=-0.5;
	des_vr=-0.5;
	
	_delay_ms(22001);
	des_vl=0;
	des_vr=0;
	
	_delay_ms(22001);
	par.posx =43;
	par.dir=3*PI/2;
	L_ENKODER = 0;
	R_ENKODER = 0;
	forward(par.posx+30,0);
	
	
	
	kal_count = 0;
	
}








void kalibruj()
{	
	if ((kal_count > 2) &&   (((labyrinth[labposx_real][labposy_real]&UP)==0)||((labyrinth[labposx_real][labposy_real]&DOWN)==0))  &&   (((labyrinth[labposx_real][labposy_real]&LEFT)==0)||((labyrinth[labposx_real][labposy_real]&RIGHT)==0))  )
	{
		
		if ((labyrinth[labposx_real][labposy_real]&turn2(INVERS))==0)
		{
			kalibruj_ruchy();
			if (kali_obrot==1)
			{
				rotateAngle(PI/2);
				mouse_dir_real=turn2(RIGHT);
			}
			
			
			else
			{
				
			
			rotateAngle(-PI/2);
			mouse_dir_real=turn2(LEFT);
			}
		}
		
		else if ((labyrinth[labposx_real][labposy_real]&turn2(RIGHT))==0)
		{rotateAngle(-PI/2);
			mouse_dir_real=turn2(LEFT);
			kalibruj_ruchy();
			rotateAngle(-PI);
			mouse_dir_real=turn2(INVERS);
		}
		
		else
		{rotateAngle(PI/2);
			mouse_dir_real=turn2(RIGHT);
			kalibruj_ruchy();
			rotateAngle(-PI);
			mouse_dir_real=turn2(INVERS);
		}
		
		
		
		
	}
	
}

