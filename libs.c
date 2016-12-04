/*
 * motors.c
 *biblioteka do oblugi silnikow
 * Created: 2016-02-28 23:13:39
 *  Author: Dominik
 */ 
#define  F_CPU    32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "libs.h"
#include "hd44780.h"
#include "gyro.h"

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
void OscXtal(void) {
	// konfiguracja generatora kwarcowego
	OSC.XOSCCTRL	=	OSC_FRQRANGE_12TO16_gc |		// wybór kwarcu od 12 do 16 MHZ
						OSC_XOSCSEL_XTAL_16KCLK_gc;		// czas na uruchomienie generatora
	OSC.CTRL		=	OSC_XOSCEN_bm;					// uruchomienie generatora kwarcowego
	// czekanie na ustabilizowanie siê generatora
	for(uint8_t i=0; i<255; i++) {
		if(OSC.STATUS & OSC_XOSCRDY_bm) {
			_delay_ms(20);
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie zmiany Ÿród³a sygna³u zegarowego
			CLK.CTRL		=	CLK_SCLKSEL_XOSC_gc;	// wybór Ÿród³a sygna³u zegarowego na XTAL 16MHz
			// uk³ad nadzoruj¹cy kwarc
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie modyfikacji wa¿nych rejestrów
			OSC.XOSCFAIL	=	OSC_XOSCFDEN_bm;		// w³¹czenie uk³adu detekcji b³êdu sygna³u zegarowego
			
			return;										// wyjœcie z funkcji jeœli generator siê uruchomi³
		}
		_delay_us(10);
	}
	//komunikat w przypadku braku uruchomienia generatora kwarcowego
	LcdClear();
	Lcd("Brak XTAL");
}
void Osc32MHz(void) {
	OSC.CTRL     =    OSC_RC32MEN_bm;       // w³¹czenie oscylatora 32MHz
	while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // czekanie na ustabilizowanie siê generatora
	CPU_CCP      =    CCP_IOREG_gc;         // odblokowanie zmiany Ÿród³a sygna³u
	CLK.CTRL     =    CLK_SCLKSEL_RC32M_gc; // zmiana Ÿród³a sygna³u na RC 32MHz
	LcdClear();                             // czyszczenie wyœwietlacza
	Lcd("Wewn RC 32MHz");                   // komunikat o uruchomieniu generatora
	_delay_ms(1000);
	LcdClear();
}

void setADC(void){	//konfuguracja adc

	Osc32MHz();
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

uint16_t debancer (uint16_t p1,uint16_t p2,uint16_t p3)
{	if(p1==p2 ||p2==p3)
	
	return p1;
	else return (-1);
}

uint16_t adcPomiar_RD(void){ //pomiar adc RD
	uint16_t a [10];
	uint16_t i=0;
	uint16_t wynik=0;
	for (i=0;i<10;i++)
	{

		PORTE.OUTSET		=	PIN4_bm;// Start ADC conversion
		//0_delay_us(15);
		ADCB.CH3.CTRL		|=	ADC_CH_START_bm;
		_delay_us(10);
		PORTE.OUTCLR		=	PIN4_bm;
		a[i]= adc_result_RD;
		if (a[i]>2500) a[i]=0;
	}
	
	for(i=0;i<10;i++)
	wynik=wynik+a[i];
	wynik=wynik/10;
	//Lcd(" RD");
	//LcdDec(wynik);
	if (wynik<350)
	return (1);
	else return (0);
	//return (wynik/10);
}
uint16_t adcPomiar_LD(void){//pomiar adc lD
	//uint16_t a [10];
	uint16_t b [10];
	uint16_t i=0;
	uint16_t wynik=0;

	
	
	for (i=0;i<10;i++)
	{
		
		PORTE.OUTSET		=	PIN7_bm;// Start ADC conversion
		
		ADCB.CH0.CTRL		|=	ADC_CH_START_bm;
		_delay_us(16);
		PORTE.OUTCLR		=	PIN7_bm;
		b[i]= adc_result_LD;
		if (b[i]>2500) b[i]=0;
	}
	for(i=0;i<10;i++)
	wynik=wynik+b[i];
	wynik=wynik/10;
	if (wynik<450)
	return (1);
	else return (0);
	
	
}



uint16_t adcPomiar_LF(void){ //pomiar adc LF
	
	uint16_t a [10];
	uint16_t i=0;
	uint16_t wynik=0;
	for (i=0;i<10;i++)
	
	{PORTE.OUTSET		=	PIN6_bm;// Start ADC conversion
		ADCB.CH1.CTRL		|=	ADC_CH_START_bm;
		_delay_us(15);
		PORTE.OUTCLR		=	PIN6_bm;
		a[i]=adc_result_LF;
		if (a[i]>2500) a[i]=0;
	}
	
	for(i=0;i<10;i++)
	wynik=wynik+a[i];
	
wynik=wynik/10;
if (wynik<180)
	return (2);
	else if (wynik<500)
	return (1);
	else
	return (0);
}
uint16_t adcPomiar_RF(void){ //pomiar adc RF
	uint16_t a [10];
	uint16_t i=0;
	uint16_t wynik=0;
	for (i=0;i<10;i++)
	
	{
		PORTE.OUTSET		=	PIN5_bm;// Start ADC conversion
		ADCB.CH2.CTRL		|=	ADC_CH_START_bm;
		_delay_us(13);
		PORTE.OUTCLR		=	PIN5_bm;
		a[i]= adc_result_RF;
		if (a[i]>2500) a[i]=0;
	}
	
	for(i=0;i<10;i++)
	wynik = wynik+a[i];
	wynik = wynik/10;
	//Lcd(" RF");
	//LcdDec(wynik);
	
	if (wynik<190)
	return (2);
	else if (wynik<1400)
	return (1);
	else
	return (0);
}
void setMotorL(void){
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
void setMotorR(void){
	//------------ustawienia silnika 2
	PORTD.DIRSET	=	PIN2_bm|//inpu2
	PIN1_bm|//input1
	PIN0_bm;//pwm
	PORTD.OUTCLR	=	PIN1_bm;//input 1->0
	PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	TCD0.CCA		=	0;
}

void runR(int8_t o, int8_t k){ //kierowanie silnikiem prawym
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
void runL(int8_t o, int8_t k){ //kierowanie silnikiem lewym
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

void setall(void){
	// ============================		wejscia		===========================================================================================
	
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
	
	setADC();
	setbat();
	LcdInit();
	
	TWI_MasterInit();
	enableDefault();
	
	
	TCD1.INTCTRLA     =    TC_OVFINTLVL_HI_gc;         // przepe³nienie ma generowaæ przerwanie LO
	
	TCD1.CTRLB        =    TC_WGMODE_NORMAL_gc;        // tryb normalny
	TCD1.CTRLA        =    TC_CLKSEL_DIV1024_gc;
	TCD1.PER = 160;
	sei();
	
}
void ledYellow(void){
	PORTF_OUTTGL=PIN5_bm;
}

void ledGreen(void){
	PORTF_OUTTGL=PIN6_bm;
}
void setbat(void){//funkcja ustawiajaca przerwanie na za niski poziom baterii
	// konfiguracja komparatora 0 w porcie A
	PORTF_OUTSET=PIN7_bm;
	ACA.AC0MUXCTRL		=	AC_MUXPOS_PIN2_gc |
	AC_MUXNEG_SCALER_gc;    // wejœcie + PIN A6
	ACA.AC0CTRL			=	AC_ENABLE_bm|AC_HYSMODE_SMALL_gc|AC_INTLVL_LO_gc|AC_INTMODE_FALLING_gc;
	ACA.CTRLB			=	45;                    // pocz¹tkowe ustawienie dzielnika napiêcia
	ACA.CTRLA			=	AC_AC0OUT_bm;
}

