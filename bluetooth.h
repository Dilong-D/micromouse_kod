/*
 * bluetooth.h
 *biblioteka do oblugi interfejsu bluetooth
 *  Author: Artur Hadasz
 */ 

void bluetooth_init();
void send_char(char c);
void send_uint16(uint16_t i);
char receiveByte();