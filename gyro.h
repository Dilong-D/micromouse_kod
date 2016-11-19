
/*
 ** gyro.h
 ** biblioteka do zyroskopu
 ** Created: 2016-08-01
 ** Author: Mateusz
 */
/* ---------------------------------------------------- */


#ifndef GYRO_H_
#define GYRO_H_

#define TWI_MASTER       TWIE
#define TWI_MASTER_PORT  PORTE
#define TWI_MASTER_ADDR  0xD7
#define TWI_SPEED        1000000

#define Taktfreuqenz 32000000
#define Takt_TWI 400000
#define TWI_BAUD(F_SYS, F_TWI) ((F_SYS / (2 * F_TWI)) - 5)
#define TWI_BAUDRATE TWI_BAUD(Taktfreuqenz, Takt_TWI)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "L3GD20H.h"
#include "hd44780.h"
#include "libs.h"

//struktura w ktorej zapisywane sa bezposrednie pomiary (8-bitowe) dane pobrane z zyroskopa
struct gyro {
	
	//int64_t xlg;
	//int64_t xhg;
	int64_t ylg;
	int64_t yhg;
	//int64_t zlg;
	//int64_t zhg;
};

//struktura w ktorej zapisane sa juz przetworzone dane (16-bitowe)
struct gyro_xyz {
	
	//int64_t x;
	int64_t y;
	//int64_t z;
	
};

void TWI_MasterInit(void); //funkcja ustawia podstawowe parametry I2C (TWI) 
void enableDefault(void); //funkcja uruchamia zyroskop i ustawia podstawowe parametry
float getAngleRadians(void);

uint8_t writeByte(uint8_t addr,uint8_t val); //funkcja zmienia wartosci(8bitow) podanego rejestru (zapisanego w postaci bajtu)
uint8_t readByte(uint8_t addr); //funkcja odczytujaca wartosc(8bitow) z podanego rejestru

void readByteMulti(void); //funkcja odczytuje parametry x,y,z zyroskopu

struct gyro_xyz gyro;

#endif /* GYRO_H_ */