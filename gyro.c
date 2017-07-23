/*
 ** gyro.c
 ** biblioteka do zyroskopu
 ** Created: 2016-08-01
 ** Author: Mateusz
 */
/* ---------------------------------------------------- */

#define  F_CPU 32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "gyro.h"



void TWI_MasterInit(void) 
{
	
	//WIEN,RIEN, ENABLE and INTLVL bits high.  0xF8 - 1111 1000 
	TWIE.MASTER.CTRLA	=	TWI_MASTER_ENABLE_bm | TWI_MASTER_RIEN_bm |	TWI_MASTER_WIEN_bm ;
	
	// SMEN - SMART MODE ENABLE - 0x01 */
	TWIE.MASTER.CTRLB	=   TWI_MASTER_SMEN_bm;
	
	//Force TWI bus into idle mode - 0x01
	TWIE.MASTER.STATUS	=	TWI_MASTER_BUSSTATE_IDLE_gc;
	
	//Set TWI baud rate
	TWIE.MASTER.BAUD	=	TWI_BAUDRATE; //0x35 - 400kHz
	
	PORTE.PIN0CTRL		=	PORT_OPC_WIREDANDPULL_gc;
	PORTE.PIN1CTRL		=	PORT_OPC_WIREDANDPULL_gc;
	
	
	//PORTE.DIR &= 0xFC;

}

uint8_t writeByte(uint8_t addr,uint8_t val);

/**************** STARTUP SEQUENCE ***************
 1. Write CTRL2
 2. Write CTRL3
 3. Write CTRL4
 4. Write CTRL6
 5. Write Reference
 6. Write IG_THS
 7. Write IG_DURATION
 8. Write IG_CFG
 9. Write CTRL5
 10. Write CTRL1
 
 ************************************************/

void enableDefault(void)
{
	
	
	// High-pass filter disabled
	writeByte(REG_CTRL2, 0x00);
	
	//CTRL3 0x80 = 0b1000000
	//INT1_IG (Interrupt enable)
	writeByte(REG_CTRL3, 0x80);
	
	// 0x20 = 0b10100000
	// FS = 10 (+/- 2000 dps full scale) and block data update = 1
	writeByte(REG_CTRL4, 0xA0);
	
	//threshold 60dps (High)
	writeByte(REG_IG_THS_YH,0x1F);
	
	//threshold 60dps (High)
	writeByte(REG_IG_THS_YL,0x58);
	
	// duration = 1/DR = 1/800 = 1.25ms
	writeByte(REG_IG_DURATION, 0x01);
	
	//Enable interrupt generation on Y high event
	writeByte(REG_IG_CFG,0x08);
	
	// 0x6F = 0b11111111
	// DR = 11 (800 Hz ODR); BW = 11 (100 Hz bandwidth); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
	writeByte(REG_CTRL1, 0xFF);
	
}

uint8_t writeByte(uint8_t addr,uint8_t val)
{
	TWIE.MASTER.ADDR = 0xD6;	//0b11010110 - 0xD6 - Write - SP + SAD + W
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	//if(!(TWIE.MASTER.STATUS & TWI_MASTER_RXACK_bm)) // SAK
	//	return 0;
	
	TWIE.MASTER.DATA = addr; // SUB
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	//if(!(TWIE.MASTER.STATUS & TWI_MASTER_RXACK_bm)) // SAK
	//	return 0;
	
	TWIE.MASTER.DATA = val; //DATA
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	TWIE.MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc; // SP
	
	return 1;
	
}

uint8_t readByte(uint8_t addr)
{
	uint8_t data = 0;
	
	TWIE.MASTER.ADDR = 0b11010110; //Write - ST + (SAD + W)
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	TWIE.MASTER.DATA = addr; //SUB
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_WIF_bm));
	
	TWIE.MASTER.ADDR = 0b11010111; //Read SR + (SAD + R)
	while(!(TWIE.MASTER.STATUS & TWI_MASTER_RIF_bm));
	
	TWIE.MASTER.CTRLC |= TWI_MASTER_ACKACT_bm;
	
	data = TWIE.MASTER.DATA;
	
	//TWIE.MASTER.CTRLC |= TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;		// send NACK

	return data;
	
}

void readByteMulti(void)
{
	struct gyro g;
	
	//g.xlg = readByte(REG_OUT_X_L);
	//g.xhg = readByte(REG_OUT_X_H);
	g.ylg = readByte(REG_OUT_Y_L);
	g.yhg = readByte(REG_OUT_Y_H);
	//g.zlg = readByte(REG_OUT_Z_L);
	//g.zhg = readByte(REG_OUT_Z_H);

	//int64_t temp;
	//if(g.xhg > 127)
	//{
		//temp = g.xhg - 127;
		//gyro.x = g.xlg + temp*256 - 32768;
	//}
	//else
	//{
		//gyro.x = g.xlg + g.xhg*256;
	//}
	
	//if(g.yhg > 127)
	//{
		//temp = g.yhg - 127;
		//gyro.y = g.ylg + temp*256 - 32768;
	//}
	//else
	//{
		//gyro.y = g.ylg + g.yhg*256;
	//}
	
	//if(g.zhg > 127)
	//{
		//temp = g.zhg - 127;
		//gyro.z = g.zlg + temp*256 - 32768;
	//}
	//else
	//{
		//gyro.z = g.zlg + g.zhg*256;
	//}
	gyro.y = g.ylg + g.yhg*256;
}

float getAngleRadians(){
	readByteMulti();	
	return gyro.y;//*SENSITIVITY_500DPS*DPS_TO_RADS - 0.051;
}