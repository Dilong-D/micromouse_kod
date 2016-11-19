/*
 * pomiar.c
 *
 * Created: 18.10.2016 18:42:49
 *  Author: LPA1
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pomiar.h"
#include "algorithms.h"
#include "libs.h"

 
	
short turn2(short direction){
	
		if(direction == RIGHT){
			switch(mouse_dir_real){
				case UP:
				return RIGHT;
				
				case DOWN:
				return LEFT;
				
				case RIGHT:
				return DOWN;
				
				case LEFT:
				return UP;
			}
		}
		if(direction == LEFT){
			switch(mouse_dir_real){
				case UP:
				return LEFT;
				
				case DOWN:
				return RIGHT;
				
				case RIGHT:
				return UP;
				
				case LEFT:
				return DOWN;
			}
		}
        if(direction == INVERS){
			switch(mouse_dir_real){
				case UP:
				return DOWN;
				
				case DOWN:
				return UP;
				
				case RIGHT:
				return LEFT;
				
				case LEFT:
				return RIGHT;
			}
		}
}

	int16_t mouse_dir_x(short dir) //  zapisuje jaka wartosc trzeba otrzymac by miec wartosc komorki przed
	{
		if(dir == RIGHT)
			return (1);
		if(dir == LEFT)
			return (-1);
		if(dir == UP)
			return (0);
		if(dir == DOWN)
			return (0);
			

	}
	int16_t mouse_dir_y(short dir)
	{
		if(dir == RIGHT)
			return (0);
		if(dir == LEFT)
			return (0);
		if(dir == UP)
			return (1);
		if(dir == DOWN)
			return (-1);
		

	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	 
		
		
		
		/*
		
		discovered[0][0]=1;
		labyrinth[0][0]=14;
		LcdDec (discovered[0][1]);
		Lcd(" ");
		LcdDec (discovered[1][1]);
		Lcd("  ");
		LcdDec (labyrinth[0][1]);
		Lcd(" ");
		LcdDec (labyrinth[1][1]);
		
		Lcd2;
		LcdDec (discovered[0][0]);
		Lcd(" ");
		LcdDec (discovered[1][0]);
		Lcd("  ");
		LcdDec (labyrinth[0][0]);
		Lcd(" ");
		LcdDec (labyrinth[1][0]);
		_delay_ms(1000);
	
	
	
	*/
	
	
	
	
		
		
        int16_t mierz ()
		// 1  - dobry pomiar
		// 0  - brak pomiaru
		// -1 - blad
		
		{
			
		
		
		
		
		int16_t discaverdcounter=0; // zlicza czy pole juz jest discaverd (przy discaverdcounter==3)
		 int16_t debRF;
		 int16_t debLF;
		 int16_t debRD;
		 int16_t debLD;
		
			debLD=debancer(adcPomiar_LD(),adcPomiar_LD(),adcPomiar_LD());  // pomiar LD
			
		debRD=debancer(adcPomiar_RD(),adcPomiar_RD(),adcPomiar_RD());  // pomiar RD
			_delay_ms(1);
		debRF=debancer(adcPomiar_RF(),adcPomiar_RF(),adcPomiar_RF());  // pomiar RF
			_delay_ms(1);
		debLF=debancer(adcPomiar_LF(),adcPomiar_LF(),adcPomiar_LF());  // pomiar LF
			_delay_ms(1);	
	
		
		if(((debLF!=0)&&(debRF!=0))&&(discovered[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]==0))
		{
			

		//------------------------------------------------------------------------------------------------------------------------------------------RD	
		if(debRD==-1) 
		return (-1);
		else if(debRD==0)
		
			{
			//if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&turn(RIGHT)==turn(RIGHT))
			// ;
			// else
				 {	 
					labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&=(~turn2(RIGHT));
					
					discaverdcounter++;
				 }
			}
			
		
		
	    else if(debRD==1) 
         {   //if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&turn2(RIGHT)==turn2(RIGHT))
			 //;
			 //else
			 {
				 
			 
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]|=turn2(RIGHT);    
				discaverdcounter++;
						 
			}			 
		 }
		
		
		
		//------------------------------------------------------------------------------------------------------------------------------------------RF i LF
		
		
		
		
		
			if(((debRF==-1)||(debRF==-1))|| (debRF!=debLF))
			return (-1);
			else if(debRF==0)
			{
				// if(labyrinth[labposx_real][labposy_real]&mouse_dir_real)
				// ;
				// else
				
				labyrinth[labposx_real][labposy_real]&=(~mouse_dir_real);
				
			}
			
			else if(debRF==1)
			{// if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_x( mouse_dir_real)]&mouse_dir_real)
				// ;
				// else
				
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&=(~mouse_dir_real);
				discaverdcounter++;
			
				
				
			}
			else if(debRF==2) 
			{// if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_x( mouse_dir_real)47]&mouse_dir_real)
				// ;
				// else
			
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]|=(mouse_dir_real);
				discaverdcounter++;
				
			}
			
		/*
		//------------------------------------------------------------------------------------------------------------------------------------------LF
		
		
		

		if(debLF==-1) ;
		//return (-1)
		else if(debLF==0)
			
					{
						// if(labyrinth[labposx_real][labposy_real]&mouse_dir_real)
						// ;
						// else
			
					labyrinth[labposx_real][labposy_real]&=(~mouse_dir_real);
					discovered[labposx_real][labposy_real]=1;
					discaverdcounter++;
					
					}
			
		else if(debLF==1)
		{// if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_x( mouse_dir_real)]&mouse_dir_real)
			// ;
			// else
			
			labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&=(~mouse_dir_real);
			discaverdcounter++;
			
			
		} 
		else if(debLF==2) ;
			{// if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_x( mouse_dir_real)47]&mouse_dir_real)
				// ;
				// else
				
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&=(mouse_dir_real);
				discaverdcounter++;
				
			}
	   
	   
	   
	   */
			//------------------------------------------------------------------------------------------------------------------------------------------LD
		if(debLD==-1) 
		return (-1);
		else if(debLD==0)
		{
			// if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&turn(LEFT)==turn(LEFT))
			// ;
			// else
			{
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&=(~turn2(LEFT));
				discaverdcounter++;
				
			}
		}
		
		else if(debLD==1)
		{  // if(labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]&turn2(LEFT)==turn2(LEFT))
			//;
		//	else
			//{
				
				
				labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]|=(turn2(LEFT));
				
				discaverdcounter++;
				
		}
		}
		else
		
		
		{   
			/*
		 	LcdDec (discovered[0][1]);
		 	Lcd(" ");
		 	LcdDec (discovered[1][1]);
		 	Lcd("  ");
		 	LcdDec (labyrinth[0][1]);
		 	Lcd(" ");
		 	LcdDec (labyrinth[1][1]);
		 	
		 	Lcd2;
		 	LcdDec (discovered[0][0]);
		 	Lcd(" ");
		 	LcdDec (discovered[1][0]);
		 	Lcd("  ");
		 	LcdDec (labyrinth[0][0]);
		 	Lcd(" ");
		 	LcdDec (labyrinth[1][0]);
		 	_delay_ms(1000);
		 *//*
		 Lcd2;
		 Lcd("pomiar niepotrzebny ");*/
		 return 0;
		 
		}
		
				if (discaverdcounter==3)
						if(discovered[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]==0)
							{labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]|= turn2(INVERS) ;
							new_wall_discovered=1;
							discovered[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]=1;
					/*	Lcd2;
					Lcd("z ");
					LcdDec (labyrinth[labposx_real+mouse_dir_x( mouse_dir_real)][labposy_real+mouse_dir_y( mouse_dir_real)]);
					Lcd(" ");	
					LcdDec(labposx_real+mouse_dir_x( mouse_dir_real));
					Lcd(" ");
					LcdDec(labposy_real+mouse_dir_y( mouse_dir_real));*/
						
					/*	
					LcdDec (discovered[0][1]);
					Lcd(" ");
					LcdDec (discovered[1][1]);
					Lcd("  ");
					LcdDec (labyrinth[0][1]);
					Lcd(" ");
					LcdDec (labyrinth[1][1]);
					
					Lcd2;
					LcdDec (discovered[0][0]);
					Lcd(" ");
					LcdDec (discovered[1][0]);
					Lcd("  ");
					LcdDec (labyrinth[0][0]);
					Lcd(" ");
					LcdDec (labyrinth[1][0]);
					_delay_ms(1000);
					*/
		
		
					 return 1;

	
	
	   
			}
	return(-1);

		}
	
