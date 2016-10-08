/*
 ** gyro.c
 ** biblioteka do zyroskopu
 ** Created: 2016-08-01
 ** Author: Mateusz
 */
/* ---------------------------------------------------- */

#define  F_CPU    2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "gyro.h"



void TWI_MasterInit(void) 
{
	
	/*WIEN,RIEN, ENABLE and INTLVL bits high.  0xF8 - 1111 1000 */
	TWIE.MASTER.CTRLA	=	TWI_MASTER_ENABLE_bm |
	TWI_MASTER_INTLVL_HI_gc |
	TWI_MASTER_RIEN_bm |
	TWI_MASTER_WIEN_bm;
	/* SMEN - SMART MODE ENABLE - 0x01 */
	TWIE.MASTER.CTRLB	=   TWI_MASTER_SMEN_bm;
	
	/*Force TWI bus into idle mode - 0x01*/
	TWIE.MASTER.STATUS	=	TWI_MASTER_BUSSTATE_IDLE_gc;
	
	/*Set TWI baud rate*/
	TWIE.MASTER.BAUD	=	TWI_BAUDRATE; //0x35 - 400kHz
	
	PORTE.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTE.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;
	
	//PORTE.DIR &= 0xFC;

}

uint8_t writeByte(uint8_t addr,uint8_t val);

void enableDefault(void)
{
	// 0x6F = 0b01101111
	// DR = 01 (200 Hz ODR); BW = 10 (50 Hz bandwidth); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
	if(writeByte(REG_CTRL1, 0x6F))
	{

		LcdClear();
		Lcd("dziala ctrl1");
		//Lcd2;
		//LcdDec(t);
		_delay_ms(500);
	}
	
	// 0x20 = 0b00100000
	// FS = 10 (+/- 2000 dps full scale)
	if(writeByte(REG_CTRL4, 0x20))
	{
		
		LcdClear();
		Lcd("dziala ctrl4");
		//Lcd2;
		//LcdDec(t);
		_delay_ms(500);
		
	}
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
	
	
	data = TWIE.MASTER.DATA;
	
	
	TWIE.MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;		// send NACK

	return data;
	
}

struct gyro_xyz readByteMulti()
{
	struct gyro g;
	struct gyro_xyz xyz;
	
	g.xlg = readByte(REG_OUT_X_L);
	g.xhg = readByte(REG_OUT_X_H);
	g.ylg = readByte(REG_OUT_Y_L);
	g.yhg = readByte(REG_OUT_Y_H);
	g.zlg = readByte(REG_OUT_Z_L);
	g.zhg = readByte(REG_OUT_Z_H);

	int64_t temp;
	if(g.xhg > 127)
	{
		temp = g.xhg - 127;
		xyz.x = g.xlg + temp*256 - 32768;
	}
	else
	{
		xyz.x = g.xlg + g.xhg*256;
	}
	
	if(g.yhg > 127)
	{
		temp = g.yhg - 127;
		xyz.y = g.ylg + temp*256 - 32768;
	}
	else
	{
		xyz.y = g.ylg + g.yhg*256;
	}
	
	if(g.zhg > 127)
	{
		temp = g.zhg - 127;
		xyz.z = g.zlg + temp*256 - 32768;
	}
	else
	{
		xyz.z = g.zlg + g.zhg*256;
	}
	
	return xyz;
	
}

