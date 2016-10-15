/*
 * motors.c
 *biblioteka do oblugi silnikow
 * Created: 2016-02-28 23:13:39
 *  Author: Dominik
 */ 
#define  F_CPU    2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "libs.h"
#include "hd44780.h"

uint8_t ReadCalibrationByte( uint8_t index ) //konfuguracja adc
{
	uint8_t result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);

	/* Clean up NVM Command register. */
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return( result );
}


void setADC(){	//konfuguracja adc
	OSC.PLLCTRL = OSC_PLLSRC_RC2M_gc | OSC_PLLFAC3_bm; // 0x08 - wybieramy RC 2Mhz RC jako ?ród?o, a mno?nik PLL = 8
	OSC.CTRL |= OSC_PLLEN_bm; // 0x10 - W??czamy PLL
	while (0 == (OSC.STATUS & OSC_PLLRDY_bm)); // Opó?nienie konieczne dla stabilizacji PLL
	CLK.CTRL = CLK_SCLKSEL2_bm; // 0x04 - ustawiamy PLL jako ?ród?o taktowania
	CLK.PSCTRL = CLK_PSADIV0_bm; // Clk_per4 = Clk_sys/2  = 16MHz/2 = 8MHz
	ADCB.PRESCALER = ADC_PRESCALER2_bm; // 0x04
	ADCB.CTRLB = ADC_CONMODE_bm; // 0x10
	ADCB.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
	ADCB.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );
	ADCB.REFCTRL = ADC_REFSEL0_bm; // 0x10
	ADCB.EVCTRL = 0x00 ;
	ADCB.CTRLA = ADC_ENABLE_bm; // 0x01
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	//-------------setADC_LD
	PORTB.DIRCLR = PIN1_bm;// Ustawiamy PB1 jako wej?cie
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE0_bm; // 0x01
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCB.INTFLAGS = ADC_CH0IF_bm; // 0x01
	ADCB.CH0.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm; // 0x03
	//-------------setADC_RD
	PORTB.DIRCLR = PIN0_bm;// Ustawiamy PB0 jako wej?cie
	ADCB.CH1.CTRL = ADC_CH_INPUTMODE0_bm; // 0x01
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
	ADCB.INTFLAGS = ADC_CH1IF_bm; // 0x01
	ADCB.CH1.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm; // 0x03
	//-------------setADC_LF
	PORTB.DIRCLR = PIN2_bm;// Ustawiamy PB1 jako wej?cie
	ADCB.CH2.CTRL = ADC_CH_INPUTMODE0_bm; // 0x01
	ADCB.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
	ADCB.INTFLAGS = ADC_CH2IF_bm; // 0x01
	ADCB.CH2.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm; // 0x03
	//-------------setADC_RF
	PORTB.DIRCLR = PIN3_bm;// Ustawiamy PB1 jako wej?cie
	ADCB.CH3.CTRL = ADC_CH_INPUTMODE0_bm; // 0x01
	ADCB.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
	ADCB.INTFLAGS = ADC_CH3IF_bm; // 0x01
	ADCB.CH3.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm; // 0x03
	sei();	
}
uint16_t adcPomiar_RD(){ //pomiar adc RD
	PORTE.OUTSET		=	PIN4_bm;// Start ADC conversion
	ADCB.CH3.CTRL		|=	ADC_CH_START_bm;
	_delay_us(100);
	PORTE.OUTCLR		=	PIN4_bm;
	return adc_result_RD;
}
uint16_t adcPomiar_LD(){//pomiar adc lD
	PORTE.OUTSET		=	PIN7_bm;// Start ADC conversion
	ADCB.CH0.CTRL		|=	ADC_CH_START_bm;
	_delay_us(100);
	PORTE.OUTCLR		=	PIN7_bm;
	return adc_result_LD;
}
uint16_t adcPomiar_LF(){ //pomiar adc LF
	PORTE.OUTSET		=	PIN6_bm;// Start ADC conversion
	ADCB.CH1.CTRL		|=	ADC_CH_START_bm;
	_delay_us(100);
	PORTE.OUTCLR		=	PIN6_bm;
	return adc_result_LF;
}
uint16_t adcPomiar_RF(){ //pomiar adc RF
	PORTE.OUTSET		=	PIN5_bm;// Start ADC conversion
	ADCB.CH2.CTRL		|=	ADC_CH_START_bm;
	_delay_us(100);
	PORTE.OUTCLR		=	PIN5_bm;
	return adc_result_RF;
}
void setMotorL(){
	//------------ustawienia silnika 1
	PORTD.DIRSET	=	PIN5_bm|//inpu2
						PIN4_bm|//input1
						PIN3_bm;//pwm
	PORTD.OUTCLR	=	PIN5_bm;//input 1->0
	PORTD.OUTCLR	=	PIN4_bm;//input 2->0
	TCD0.CTRLB		=	TC_WGMODE_SINGLESLOPE_gc|		// pwm singleslope
									TC0_CCDEN_bm|
									TC0_CCAEN_bm;
	TCD0.PER		=	100;
	TCD0.CCD		=	0;
	TCD0.CTRLA		=	TC_CLKSEL_DIV1_gc;
}
void setMotorR(){
	//------------ustawienia silnika 2
	PORTD.DIRSET	=	PIN2_bm|//inpu2
	PIN1_bm|//input1
	PIN0_bm;//pwm
	PORTD.OUTCLR	=	PIN1_bm;//input 1->0
	PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	TCD0.CCA		=	0;
}

void runL(int8_t o, int8_t k){ //kierowanie silnikiem L
	TCD0.CCD		=	o;
	if(k==LUZ){
		PORTD.OUTCLR	=	PIN5_bm;//input 1->0
		PORTD.OUTCLR	=	PIN4_bm;//input 2->0
	}
	else if(k==STOP){
		PORTD.OUTSET	=	PIN5_bm;//input 1->1
		PORTD.OUTSET	=	PIN4_bm;//input 2->1
	}
	else if(k==PRAWO){
		PORTD.OUTCLR	=	PIN5_bm;//input 1->0
		PORTD.OUTSET	=	PIN4_bm;//input 2->1
	}
	else{
		PORTD.OUTSET	=	PIN5_bm;//input 1->1
		PORTD.OUTCLR	=	PIN4_bm;//input 2->0		
	}
}
void runR(int8_t o, int8_t k){ //kierowanie silnikiem R
	TCD0.CCA		=	o;
	if(k==LUZ){
		PORTD.OUTCLR	=	PIN1_bm;//input 1->0
		PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	}
	else if(k==STOP){
		PORTD.OUTSET	=	PIN1_bm;//input 1->1
		PORTD.OUTSET	=	PIN2_bm;//input 2->1
	}
	else if(k==PRAWO){
		PORTD.OUTCLR	=	PIN1_bm;//input 1->0
		PORTD.OUTSET	=	PIN2_bm;//input 2->1
	}
	else{
		PORTD.OUTSET	=	PIN1_bm;//input 1->1
		PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	}
}

void setall(){
	// ============================		wejscia		===========================================================================================
	sei();
	//-----------------------------		przyciski	-----------------------------------------------------------------------
	PORTF.DIRCLR		=	PIN2_bm|PIN3_bm;					// pin F2 F3 jako wejœcie
	PORTF.INT0MASK		=   PIN2_bm;               // pin F2 ma generowaæ przerwania INT0
	PORTF.INT1MASK		=   PIN3_bm;
	PORTF.PIN2CTRL		=   PORT_OPC_PULLUP_gc|    // pull-up na F2
	PORT_ISC_FALLING_gc;   // przerwanie wywo³uje zbocze opadaj¹ce
	PORTF.PIN3CTRL		=   PORT_OPC_PULLUP_gc|    // pull-up na F3
	PORT_ISC_FALLING_gc;   // przerwanie wywo³uje zbocze opadaj¹ce
	PORTF.INTCTRL		=   PORT_INT0LVL_LO_gc| PORT_INT1LVL_LO_gc;   // poziom LO dla przerwania INT0 portu F2 F3
	//-------------------------------	enkodera R	------------------------------------------------------
	PORTC.PIN0CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	PORTC.PIN1CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	EVSYS.CH0MUX		=   EVSYS_CHMUX_PORTC_PIN0_gc;        // pin C0 wywo³uje zdarzenie
	EVSYS.CH0CTRL		=   EVSYS_QDEN_bm|                    // w³¹czenie dekodera w systemie zdarzeñ
							EVSYS_DIGFILT_8SAMPLES_gc;        // filtr cyfrowy
	TCC0.CTRLA			=   TC_CLKSEL_EVCH0_gc;               // taktowanie systemem zdarzeñ
	TCC0.CTRLD			=   TC_EVACT_QDEC_gc |                // w³¹czenie dekodera kwadraturowego
							TC_EVSEL_CH0_gc;                  // dekoder zlicza impulsy z kana³u 0*/
	//-------------------------------	enkodera L	------------------------------------------------------
	PORTF.PIN0CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	PORTF.PIN1CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	EVSYS.CH2MUX		=   EVSYS_CHMUX_PORTF_PIN0_gc;        // pin E0 wywo³uje zdarzenie
	EVSYS.CH2CTRL		=   EVSYS_QDEN_bm|                    // w³¹czenie dekodera w systemie zdarzeñ
							EVSYS_DIGFILT_8SAMPLES_gc;        // filtr cyfrowy
	TCF0.CTRLA			=   TC_CLKSEL_EVCH2_gc;               // taktowanie systemem zdarzeñ
	TCF0.CTRLD			=   TC_EVACT_QDEC_gc |                // w³¹czenie dekodera kwadraturowego
							TC_EVSEL_CH2_gc;                  // dekoder zlicza impulsy z kana³u 0*/	
	//==============================	wyjscia		=========================================================
	setMotorL();
	setMotorR();
	//-----------------------------------diody IR--------------------------------------
	PORTE.DIRSET		=	PIN5_bm|
							PIN4_bm|
							PIN6_bm|
							PIN7_bm;
	PORTE.OUTCLR		=	PIN5_bm|
							PIN4_bm|
							PIN6_bm|
							PIN7_bm;
//-----------------------------------diody LED--------------------------------------
	PORTF.DIRSET		=	PIN5_bm|
							PIN6_bm|
							PIN7_bm;
	PORTF.OUTCLR		=	PIN5_bm|
							PIN6_bm|
							PIN7_bm;  
	// ----------------------------		LCD		------------------------------
	LcdInit();
	setADC();
	setbat();
	TCC1.INTCTRLA     =    TC_OVFINTLVL_LO_gc;         // przepe³nienie ma generowaæ przerwanie LO
	
	TCC1.CTRLB        =    TC_WGMODE_NORMAL_gc;        // tryb normalny
	TCC1.CTRLA        =    TC_CLKSEL_DIV1024_gc;
	TCC1.PER=2*1024;
	
}
void ledYellow(){
	PORTF_OUTTGL=PIN5_bm;
}

void ledGreen(){
	PORTF_OUTTGL=PIN6_bm;
}
void setbat(){//funkcja ustawiajaca przerwanie na za niski poziom baterii
	// konfiguracja komparatora 0 w porcie A
	ACA.AC0MUXCTRL		=	AC_MUXPOS_PIN2_gc |
	AC_MUXNEG_SCALER_gc;    // wejœcie + PIN A6
	ACA.AC0CTRL			=	AC_ENABLE_bm|AC_HYSMODE_SMALL_gc|AC_INTLVL_LO_gc|AC_INTMODE_FALLING_gc;
	ACA.CTRLB			=	45;                    // pocz¹tkowe ustawienie dzielnika napiêcia
	ACA.CTRLA			=	AC_AC0OUT_bm;
}

