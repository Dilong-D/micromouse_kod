/*
 * L3GD20H LIBRARY
 *
 * Created: 2016-04-09 
 * Author : Mateusz W¹sala
 */ 
#ifndef __L3GD20H_H__
#define __L3GD20H_H__


#define L3GD20H_ADDRESS				0x6B				// 1101011

#define SENSITIVITY_245DPS			(0.00875F)			// Roughly 22/256 for fixed point match
#define SENSITIVITY_500DPS			(0.0175F)			// Roughly 45/256
#define SENSITIVITY_2000DPS			(0.070F)			// Roughly 18/256
#define DPS_TO_RADS					(0.017453293F)		// degres/s to rad/s multiplier

/*
==================================================================================================
	L3GD20H REG
==================================================================================================
*/
										  // DEFAULT    TYPE
#define REG_WHO_AM_I             (0x0F)   // 11010100   r
#define REG_CTRL1				 (0x20)   // 00000111   rw
#define REG_CTRL2				 (0x21)   // 00000000   rw
#define REG_CTRL3				 (0x22)   // 00000000   rw
#define REG_CTRL4				 (0x23)   // 00000000   rw
#define REG_CTRL5				 (0x24)   // 00000000   rw
#define REG_REFERENCE            (0x25)   // 00000000   rw
#define REG_OUT_TEMP             (0x26)   //            r
#define REG_STATUS		         (0x27)   //            r
#define REG_OUT_X_L              (0x28)   //            r
#define REG_OUT_X_H              (0x29)   //            r
#define REG_OUT_Y_L              (0x2A)   //            r
#define REG_OUT_Y_H              (0x2B)   //            r
#define REG_OUT_Z_L              (0x2C)   //            r
#define REG_OUT_Z_H              (0x2D)   //            r
#define REG_FIFO_CTRL			 (0x2E)   // 00000000   rw
#define REG_FIFO_SRC			 (0x2F)   //            r
#define REG_IG_CFG				 (0x30)   // 00000000   rw
#define REG_IG_SRC               (0x31)   //            r
#define REG_IG_TSH_XH            (0x32)   // 00000000   rw
#define REG_IG_TSH_XL            (0x33)   // 00000000   rw
#define REG_IG_TSH_YH            (0x34)   // 00000000   rw
#define REG_IG_TSH_YL            (0x35)   // 00000000   rw
#define REG_IG_TSH_ZH            (0x36)   // 00000000   rw
#define REG_IG_TSH_ZL            (0x37)   // 00000000   rw
#define REG_IG_DURATION          (0x38)   // 00000000   rw
#define REG_LOW_ODR				 (0x39)   // --000000

/*===============================================================================================*/

/*
======================================================================
	CTRL1 REG: | DR1 | DR0 | BW1 | BW0 | PD | Zen | Xen | Yen |
======================================================================
*/

/* Output data rate selection (DR1-DR0) */

typedef enum CTRL1_OUTPUT_DATA_RATE_enum
{
	
	CTRL1_OUTPUT_DATA_RATE_0_gc		 = (0x00<<6),		
	CTRL1_OUTPUT_DATA_RATE_1_gc		 = (0x01<<6),		
	CTRL1_OUTPUT_DATA_RATE_2_gc		 = (0x02<<6),		
	CTRL1_OUTPUT_DATA_RATE_3_gc		 = (0x03<<6)		
	
} CTRL1_OUTPUT_DATA_RATE_t;

/* Bandwidth selection (BW1-BW0) */
typedef enum CTRL1_BANDWIDTH_enum
{
	CTRL1_BANDWIDTH_0_gc			 = (0x00<<4),
	CTRL1_BANDWIDTH_1_gc			 = (0x01<<4),
	CTRL1_BANDWIDTH_2_gc			 = (0x02<<4),
	CTRL1_BANDWIDTH_3_gc			 = (0x03<<4)
	
} CTRL1_BANDWIDTH_t;

/* Power mode (PD) */
typedef enum CTRL1_POWER_MODE_enum
{
	CTRL1_POWER_MODE_0_gc			 = (0x00<<3),		/* Power Down */
	CTRL1_POWER_MODE_1_gc			 = (0x01<<3)		/* Normal Mode */
	/* Sleep Mode set {PD:Zen:Yen:Xen} to {1000} */
		
} CTRL1_POWER_MODE_t;


/* Z axis enable Default value: 1 */
#define CTRL1_Zen_EN				(0x01<<2)			/*  enabled */
#define CTRL1_Zen_DISEN				(0x00<<2)			/*	disabled */

/* Y axis enable Default value: 1 */
#define CTRL1_Yen_EN				(0x01<<1)			/*  enabled */
#define CTRL1_Yen_DISEN				(0x00<<1)			/*	disabled */

/* X axis enable  Default value: 1 */
#define CTRL1_Xen_EN				(0x01<<0)			/*  enabled */
#define CTRL1_Xen_DISEN				(0x00<<0)			/*	disabled */

/*===============================================================================================*/

/*
======================================================================================
	CTRL2 REG: | EXTRen | LVLen | HPM1 | HPM0 | HPCF3 | HPCF2 | HPCF1 | HPCF0 |
======================================================================================
*/

/* Edge sensitive trigger */
#define CTRL2_EXTRen_EN				(0x01<<7)			/*  External trigger enabled */
#define CTRL2_EXTRen_DISEN			(0x00<<7)			/*	external trigger disabled */

/* Level sensitive trigger */
#define CTRL2_LVLen_EN				(0x01<<6)			/*  level sensitive trigger enabled */
#define CTRL2_LVLen_DISEN			(0x00<<6)			/*	level sensitive trigger disabled */

/* High Pass filter Mode Selection (HPM1-HPM0) */
typedef enum CTRL2_HIGH_PASS_FILTER_MODE_enum 
{
	//default: 00
	CTRL2_HIGH_PASS_FILTER_MODE_0_gc		 = (0x00<<4),		/*	 Normal mode (reset reading REFERENCE 25h REG) */
	CTRL2_HIGH_PASS_FILTER_MODE_1_gc		 = (0x01<<4),		/*	 Reference signal for filtering  */
	CTRL2_HIGH_PASS_FILTER_MODE_2_gc		 = (0x02<<4),		/*	 Normal mode */
	CTRL2_HIGH_PASS_FILTER_MODE_3_gc		 = (0x03<<4)		/*	 Autoreset on interrupt event */
	
} CTRL2_HIGH_PASS_FILTER_MODE_t;

/* High Pass filter Cut Off frequency selection (HPCF3-HPCF0) */
typedef enum CTRL2_HIGH_PASS_FILTER_CUTOFF_enum 
{
	/* Dokumentacja - strona 38 */
	CTRL2_HIGH_PASS_FILTER_CUTOFF_0_gc		 = (0x00<<0),		
	CTRL2_HIGH_PASS_FILTER_CUTOFF_1_gc		 = (0x01<<0),		
	CTRL2_HIGH_PASS_FILTER_CUTOFF_2_gc		 = (0x02<<0),		
	CTRL2_HIGH_PASS_FILTER_CUTOFF_3_gc		 = (0x03<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_4_gc		 = (0x04<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_5_gc		 = (0x05<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_6_gc		 = (0x06<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_7_gc		 = (0x07<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_8_gc		 = (0x08<<0),
	CTRL2_HIGH_PASS_FILTER_CUTOFF_9_gc		 = (0x09<<0)
	
} CTRL2_HIGH_PASS_FILTER_CUTOFF_t;

/*===============================================================================================*/

/*
=================================================================================================================
	CTRL3 REG: | INT1_IG | INT1_Boot | H_Lactive | PP_OD | INT2_DRDY | INT2_FTH | INT2_ORun | INT2_Empty |
=================================================================================================================
*/

/* Interrupt enable on INT1 pin */
#define CTRL3_INT1_IG_EN			(0x01<<7)			/*  enabled */
#define CTRL3_INT1_IG_DISEN			(0x00<<7)			/*	disabled */

/* Boot status available on INT1 pin. */
#define CTRL3_INT1_Boot_EN			(0x01<<6)			/*  enabled */
#define CTRL3_INT1_Boot_DISEN		(0x00<<6)			/*	disabled */

/* Interrupt active configuration on INT. */
#define CTRL3_H_Lactive_HIGH		(0x00<<5)			/*  enabled */
#define CTRL3_H_Lactive_LOW			(0x01<<5)			/*	disabled */

/* Push- Pull / Open drain. */
#define CTRL3_PP_OD_PUSH_PULL		(0x00<<4)			/*  push-pull */
#define CTRL3_PP_OD_OPEN_DRAIN		(0x01<<4)			/*	open drain */

/* Date Ready on DRDY/INT2 pin. */
#define CTRL3_INT2_DRDY_EN			(0x01<<3)			/*  enabled */
#define CTRL3_INT2_DRDY_DISEN		(0x00<<3)			/*	disabled */

/* FIFO Threshold interrupt on DRDY/INT2 pin. */
#define CTRL3_INT2_FTH_EN			(0x01<<2)			/*  enabled */
#define CTRL3_INT2_FTH_DISEN		(0x00<<2)			/*	disabled */

/* FIFO Overrun interrupt on DRDY/INT2 pin. */
#define CTRL3_INT2_ORun_EN			(0x01<<1)			/*  enabled */
#define CTRL3_INT2_ORun_DISEN		(0x00<<1)			/*	disabled */

/* FIFO Empty interrupt on DRDY/INT2 pin */
#define CTRL3_INT2_Empty_EN			(0x01<<0)			/*  enabled */
#define CTRL3_INT2_Empty_DISEN		(0x00<<0)			/*	disabled */

/*===============================================================================================*/

/*
=========================================================================
	CTRL4 REG: | BDU | BLE | FS1 | FS0 | IMPen | ST2 | ST1 | SIM |
=========================================================================
*/

/* Block data update (BDU) */
#define CTRL4_BDU_0					(0x00<<7)			/*  continuos update */
#define CTRL4_BDU_1					(0x01<<7)			/*  output REGs not updated until MSB and LSB reading */

/* Big/little endian data selection (BLE) */
#define CTRL4_BLE_0					(0x00<<6)			/*  Data LSB @ lower address */
#define CTRL4_BLE_1					(0x01<<6)			/*  Data MSB @ lower address */

/* Sensitivity setting (FS1-FS0) */
typedef enum CTRL4_SENSITIVITY_enum 
{
	
	CTRL4_SENSITIVITY_245DPS_gc		 = (0x00<<4),		/*	 245 dps */
	CTRL4_SENSITIVITY_500DPS_gc		 = (0x01<<4),		/*	 500 dps */
	CTRL4_SENSITIVITY_2000DPS_gc	 = (0x02<<4),		/*	2000 dps */
	
} CTRL4_SENSITIVITY_t;

/* Level sensitive latched (IMPen) */ 
#define CTRL4_IMPen_EN_bm			(0x01<<3)			/*  enabled */
#define CTRL4_IMPen_DISEN_bm		(0x00<<3)			/*	disabled */

/* Self - Test (ST2-ST1) */
typedef enum CTRL4_SELF_TEST_enum 
{
	
	CTRL4_SELF_TEST_NORMAL_MODE_gc	 = (0x00<<1),		/*  normal mode		*/
	CTRL4_SELF_TEST_0_gc			 = (0x01<<1),		/*  self-test 0(+)  */
	CTRL4_SELF_TEST_UNUSED_gc		 = (0x02<<1),		/*  unused			*/
	CTRL4_SELF_TEST_1_gc			 = (0x03<<1)		/*  self-test 1(-)  */
	
} CTRL4_SELF_TEST_t;

/* SPI Serial Interface Mode selection (SIM) */
#define CTRL4_SPI_SIM_3_WIRE_bm		(0x00<<0)			/*  SPI 3 wire interface */
#define CTRL4_SPI_SIM_4_WIRE_bm		(0x01<<0)			/*  SPI 4 wire interface */

/*===============================================================================================*/

/*
======================================================================================================
	CTRL5 REG: | BOOT | FIFO_EN | StopOnFTH | HPen | IG_Sel1 | IG_Sel0 | Out_Sel1 | Out_Sel0 |
======================================================================================================
*/

/* Reboot memory content */
#define CTRL5_BOOT_0_bm				(0x00<<7)			/*  normal mode */
#define CTRL5_BOOT_1_bm				(0x01<<7)			/*  reboot memory content */

/* FIFO enable, Default value: 0 */
#define CTRL5_FIFO_DISEN_bm			(0x00<<6)			/*  enabled */
#define CTRL5_FIFO_EN_bm			(0x01<<6)			/*  disabled */

/* Sensing chain FIFO stop values memorization at FIFO Threshold, Default value: 0  */
#define CTRL5_StopOnFTH_0_bm		(0x00<<5)			/*  FIFO depth is not limited (32 digital words per axis) */
#define CTRL5_StopOnFTH_1_bm		(0x01<<5)			/*  FIFO depth is limited to FIFO 
																	Threshold which is defined in FIFO_CTRL */

/* INT Generator selection configuration */
typedef enum CTRL5_IG_Sel_enum
{
	
	CTRL5_IG_Sel_0_gc				= (0x00<<2),		/*  00 */
	CTRL5_IG_Sel_1_gc				= (0x01<<2),		/*  01 */
	CTRL5_IG_Sel_2_gc				= (0x02<<2),		/*  10 */
	CTRL5_IG_Sel_3_gc				= (0x03<<2)			/*  11 */
	
} CTRL5_IG_Sel_t;


/* Out selection configuration */
typedef enum CTRL5_OUT_Sel_enum
{
	
	CTRL5_OUT_Sel_0_gc				= (0x00<<0),		/*  00 */
	CTRL5_OUT_Sel_1_gc				= (0x01<<0),		/*  01 */
	CTRL5_OUT_Sel_2_gc				= (0x02<<0),		/*  10 */
	CTRL5_OUT_Sel_3_gc				= (0x03<<0)			/*  11 */
	
} CTRL5_OUT_Sel_t;


/*===============================================================================================*/

/*
=====================================================================================================
	REFERENCE: | Ref7 | Ref6 | Ref5 | Ref4 | Ref3 | Ref2 | Ref1 | Ref0 |
=====================================================================================================
*/

/* Digital high pass filter reference value */
typedef enum REFERENCE_enum
{
	
	REFERENCE_0_bm					= (0x01<<0),		
	REFERENCE_1_bm					= (0x01<<1),		
	REFERENCE_2_bm					= (0x01<<2),		
	REFERENCE_3_bm					= (0x01<<3),			
	REFERENCE_4_bm					= (0x01<<4),			
	REFERENCE_5_bm					= (0x01<<5),			
	REFERENCE_6_bm					= (0x01<<6),		
	REFERENCE_7_bm					= (0x01<<7)			
		
} REFERENCE_t;


/*===============================================================================================*/

/*
=====================================================================================================
	OUT_TEMP: | Temp7 | Temp6 | Temp5 | Temp4 | Temp3 | Temp2 | Temp1 | Temp0 |
=====================================================================================================
*/

/* Temperature data (-1LSB/deg with 8 bit resolution). The value is expressed as two’s complement. */
typedef enum OUT_TEMP_enum
{
	
	OUT_TEMP_0_bm					= (0x01<<0),
	OUT_TEMP_1_bm					= (0x01<<1),
	OUT_TEMP_2_bm					= (0x01<<2),
	OUT_TEMP_3_bm					= (0x01<<3),
	OUT_TEMP_4_bm					= (0x01<<4),
	OUT_TEMP_5_bm					= (0x01<<5),
	OUT_TEMP_6_bm					= (0x01<<6),
	OUT_TEMP_7_bm					= (0x01<<7)
	
} OUT_TEMP_t;

/*===============================================================================================*/

/*
=====================================================================================================
	STATUS: | ZYXOR | ZOR | YOR | XOR | ZYXDA | ZDA | YDA | XDA |
=====================================================================================================
*/

/* X, Y, Z -axis data overrun. Default value: 0 */
#define STATUS_ZYXOR_0_bm				(0x00<<7)			/*  no overrun has occurred */
#define STATUS_ZYXOR_1_bm				(0x01<<7)			/*  new data has overwritten the previous 
																		one before it was read */

/* Z axis data overrun. Default value: 0 */
#define STATUS_ZOR_0_bm					(0x00<<7)			/*  no overrun has occurred */
#define STATUS_ZOR_1_bm					(0x01<<7)			/*  a new data for the Z-axis has overwritten the previous one */

/* Y axis data overrun. Default value: 0 */
#define STATUS_YOR_0_bm					(0x00<<7)			/*  no overrun has occurred */
#define STATUS_YOR_1_bm					(0x01<<7)			/*  a new data for the Y-axis has overwritten the previous one */

/* X axis data overrun. Default value: 0 */
#define STATUS_XOR_0_bm					(0x00<<7)			/*  no overrun has occurred */
#define STATUS_XOR_1_bm					(0x01<<7)			/*  a new data for the X-axis has overwritten the previous one */

/* X, Y, Z -axis new data available. Default value: 0 */
#define STATUS_ZYXDA_0_bm				(0x00<<7)			/*  a new set of data is not yet available */
#define STATUS_ZYXDA_1_bm				(0x01<<7)			/*  a new set of data is available */

/* Z axis new data available. Default value: 0 */
#define STATUS_ZDA_0_bm					(0x00<<7)			/*  a new data for the Z-axis is not yet available */
#define STATUS_ZDA_1_bm					(0x01<<7)			/*  a new data for the Z-axis is available */

/* Y axis new data available. Default value: 0 */
#define STATUS_YDA_0_bm					(0x00<<7)			/*  a new data for the Y-axis is not yet available */
#define STATUS_YDA_1_bm					(0x01<<7)			/*  a new data for the Y-axis is available */

/* X axis new data available. Default value: 0 */
#define STATUS_XDA_0_bm					(0x00<<7)			/*  a new data for the X-axis is not yet available */
#define STATUS_XDA_1_bm					(0x01<<7)			/*  a new data for the X-axis is available */

/*===============================================================================================*/

/*
=====================================================================================================
	FIFO_CTRL: | FM2 | FM1 | FM0 | FTH4 | FTH3 | FTH2 | FTH1 | FTH0 |
=====================================================================================================
*/

/* FIFO mode selection (FM2-FM0) Default value: 000 */
typedef enum FIFO_MODE_FM_enum
{
	FIFO_MODE_FM_0_gc					= (0x00<<5),			/* Bypass mode */
	FIFO_MODE_FM_1_gc					= (0x01<<5),			/* FIFO mode */
	FIFO_MODE_FM_2_gc					= (0x02<<5),			/* Stream mode */
	FIFO_MODE_FM_3_gc					= (0x03<<5),			/* Stream-to-FIFO mode */
	FIFO_MODE_FM_4_gc					= (0x04<<5),			/* Bypass-to-stream mode */
	FIFO_MODE_FM_6_gc					= (0x06<<5),			/* Dynamic stream mode */
	FIFO_MODE_FM_7_gc					= (0x07<<5)				/* Bypass-to-FIFO mode */
	
} FIFO_MODE_FM_t;

/* FIFO threshold setting. Default value: 0 (FTH4-FTH0) */
typedef enum FIFO_THRESHOLD_FTH_enum
{
	FIFO_THRESHOLD_FTH_0_bm					= (0x01<<0),			
	FIFO_THRESHOLD_FTH_1_bm					= (0x01<<1),			
	FIFO_THRESHOLD_FTH_2_bm					= (0x01<<2),			
	FIFO_THRESHOLD_FTH_3_bm					= (0x01<<3),			
	FIFO_THRESHOLD_FTH_4_bm					= (0x01<<4)			
	
	
} FIFO_THRESHOLD_FTH_t;

/*===============================================================================================*/

/*
=====================================================================================================
	FIFO_SRC: | FTH | OVRN | EMPTY | FSS4 | FSS3 | FSS2 | FSS1 | FSS0 |
=====================================================================================================
*/

/* FIFO threshold status */
#define FIFO_SRC_FTH_0_bm				(0x00<<7)			/*  FIFO filling is lower than FTH level */
#define FIFO_SRC_FTH_1_bm				(0x01<<7)			/*  FIFO filling is equal or higher than FTH level */

/* Overrun bit status. */
#define FIFO_SRC_OVRN_0_bm				(0x00<<6)			/*  FIFO is not completely filled */
#define FIFO_SRC_OVRN_1_bm				(0x01<<6)			/*  FIFO is completely filled */

/* FIFO empty bit. */
#define FIFO_SRC_EMPTY_0_bm				(0x00<<6)			/*  FIFO not empty; */
#define FIFO_SRC_EMPTY_1_bm				(0x01<<6)			/*  FIFO empty; */

/* FIFO stored data level of the unread samples (FSS4-FSS0) */


/*===============================================================================================*/

/*
=====================================================================================================
	IG_CFG: | AND/OR | LIR | ZHIE | ZLIE | YHIE | YLIE | XHIE | XLIE |
=====================================================================================================
*/

/* AND/OR combination of Interrupt events. Default value: 0 */
#define IG_CFG_AND_OR_0_bm				(0x00<<7)			/*  OR combination of interrupt events */
#define IG_CFG_AND_OR_1_bm				(0x01<<7)			/*  AND combination of interrupt events */

/* Latch Interrupt Request. Default value: 0 */
#define IG_CFG_LIR_0_bm					(0x00<<6)			/*  interrupt request not latched */
#define IG_CFG_LIR_1_bm					(0x01<<6)			/*  interrupt request latched */

/* Enable interrupt generation on Z high event. Default value: 0 */
#define IG_CFG_ZHIE_0_bm				(0x00<<5)			/*  disable interrupt request */
#define IG_CFG_ZHIE_1_bm				(0x01<<5)			/*  enable interrupt request on measured angular rate value higher than preset threshold */

/* Enable interrupt generation on Z low event. Default value: 0 */
#define IG_CFG_ZLIE_0_bm				(0x00<<4)			/*  disable interrupt request */
#define IG_CFG_ZLIE_1_bm				(0x01<<4)			/*  enable interrupt request on measured angular rate value lower than preset threshold) */

/* Enable interrupt generation on Y high event. Default value: 0 */
#define IG_CFG_YHIE_0_bm				(0x00<<3)			/*  disable interrupt request */
#define IG_CFG_YHIE_1_bm				(0x01<<3)			/*  enable interrupt request on measured angular rate value higher than preset threshold */

/* Enable interrupt generation on Y low event. Default value: 0 */
#define IG_CFG_YLIE_0_bm				(0x00<<2)			/*  disable interrupt request */
#define IG_CFG_YLIE_1_bm				(0x01<<2)			/*  enable interrupt request on measured angular rate value lower than preset threshold) */

/* Enable interrupt generation on X high event. Default value: 0 */
#define IG_CFG_XHIE_0_bm				(0x00<<1)			/*  disable interrupt request */
#define IG_CFG_XHIE_1_bm				(0x01<<1)			/*  enable interrupt request on measured angular rate value higher than preset threshold */

/* Enable interrupt generation on X low event. Default value: 0 */
#define IG_CFG_XLIE_0_bm				(0x00<<0)			/*  disable interrupt request */
#define IG_CFG_XLIE_1_bm				(0x01<<0)			/*  enable interrupt request on measured angular rate value lower than preset threshold) */

/*===============================================================================================*/


/*
=====================================================================================================
	IG_SRC: | 0 | IA | ZH | ZL | YH | YL | XH | XL |
=====================================================================================================
*/

/* Interrupt active. Default value: 0 */
#define IG_SRC_IA_0_bm					(0x00<<7)			/*  no interrupt has been generated */
#define IG_SRC_IA_1_bm					(0x01<<7)			/*  AND combination of interrupt events */

/* Z high. Default value: 0 */
#define IG_SRC_ZH_0_bm					(0x00<<6)			/*  no interrupt */
#define IG_SRC_ZH_1_bm					(0x01<<6)			/*  High event has occurred */

/* Z low. Default value: 0 */
#define IG_SRC_ZL_0_bm					(0x00<<5)			/*  no interrupt */
#define IG_SRC_ZL_1_bm					(0x01<<5)			/*  Low event has occurred */

/* Y high. Default value: 0 */
#define IG_SRC_YH_0_bm					(0x00<<4)			/*  no interrupt */
#define IG_SRC_YH_1_bm					(0x01<<4)			/*  High event has occurred */

/* Y low. Default value: 0 */
#define IG_SRC_YL_0_bm					(0x00<<3)			/*  no interrupt */
#define IG_SRC_YL_1_bm					(0x01<<3)			/*  Low event has occurred */

/* X high. Default value: 0 */
#define IG_SRC_XH_0_bm					(0x00<<2)			/*  no interrupt */
#define IG_SRC_XH_1_bm					(0x01<<2)			/*  High event has occurred */

/* X low. Default value: 0 */
#define IG_SRC_XL_0_bm					(0x00<<1)			/*  no interrupt */
#define IG_SRC_XL_1_bm					(0x01<<1)			/*  Low event has occurred */

/*===============================================================================================*/


/*
=====================================================================================================
	IG_THS_XH: | DCRM | THSX14 | THSX13 | THSX12 | THSX11 | THSX10 | THSX9 | THSX8 |
=====================================================================================================
*/

/* Interrupt generation counter mode selection. Default value: 0 */
#define IG_THS_XH_DCRM_0_bm					(0x00<<7)			/*  Reset */
#define IG_THS_XH_DCRM_1_bm					(0x01<<7)			/*  Decrement */

/* Interrupt threshold on X axis. Default value: 000 0000 */
typedef enum IG_THS_XH_THSX_enum
{
	IG_THS_XH_THSX_8_bm				= (0x01<<0),			
	IG_THS_XH_THSX_9_bm				= (0x01<<1),			
	IG_THS_XH_THSX_10_bm			= (0x01<<2),			
	IG_THS_XH_THSX_11_bm			= (0x01<<3),			
	IG_THS_XH_THSX_12_bm			= (0x01<<4),			
	IG_THS_XH_THSX_13_bm			= (0x01<<5),			
	IG_THS_XH_THSX_14_bm			= (0x01<<6)				
	
} IG_THS_XH_THSX_t;


/*===============================================================================================*/

/*
=====================================================================================================
	IG_THS_XL: | THSX7 | THSX6 | THSX5 | THSX4 | THSX3 | THSX2 | THSX1 | THSX0 |
=====================================================================================================
*/

/* Interrupt threshold on X axis. Default value: 0000 0000 */
typedef enum IG_THS_XL_THSX_enum
{
	IG_THS_XL_THSX_0_bm				= (0x01<<0),			
	IG_THS_XL_THSX_1_bm				= (0x01<<1),			
	IG_THS_XL_THSX_2_bm				= (0x01<<2),			
	IG_THS_XL_THSX_3_bm				= (0x01<<3),			
	IG_THS_XL_THSX_4_bm				= (0x01<<4),			
	IG_THS_XL_THSX_5_bm				= (0x01<<5),			
	IG_THS_XL_THSX_6_bm				= (0x01<<6),	
	IG_THS_XL_THSX_7_bm				= (0x01<<7)			
	
} IG_THS_XL_THSX_t;


/*===============================================================================================*/

/*
=====================================================================================================
	IG_THS_YH: | - | THSY14 | THSY13 | THSY12 | THSY11 | THSY10 | THSY9 | THSY8 |
=====================================================================================================
*/

/* Interrupt threshold on Y axis. Default value: 000 0000 */
typedef enum IG_THS_YH_THSY_enum
{
	IG_THS_YH_THSY_8_bm				= (0x01<<0),			
	IG_THS_YH_THSY_9_bm				= (0x01<<1),			
	IG_THS_YH_THSY_10_bm			= (0x01<<2),			
	IG_THS_YH_THSY_11_bm			= (0x01<<3),			
	IG_THS_YH_THSY_12_bm			= (0x01<<4),			
	IG_THS_YH_THSY_13_bm			= (0x01<<5),			
	IG_THS_YH_THSY_14_bm			= (0x01<<6)				
	
} IG_THS_YH_THSY_t;


/*===============================================================================================*/

/*
=====================================================================================================
	IG_THS_YL: | THSY7 | THSY6 | THSY5 | THSY4 | THSY3 | THSY2 | THSY1 | THSY0 |
=====================================================================================================
*/

/* Interrupt threshold on Y axis. Default value: 0000 0000 */
typedef enum IG_THS_YL_THSY_enum
{
	IG_THS_YL_THSY_0_bm				= (0x01<<0),			
	IG_THS_YL_THSY_1_bm				= (0x01<<1),			
	IG_THS_YL_THSY_2_bm				= (0x01<<2),			
	IG_THS_YL_THSY_3_bm				= (0x01<<3),			
	IG_THS_YL_THSY_4_bm				= (0x01<<4),			
	IG_THS_YL_THSY_5_bm				= (0x01<<5),			
	IG_THS_YL_THSY_6_bm				= (0x01<<6),
	IG_THS_YL_THSY_7_bm				= (0x01<<7)				
	
} IG_THS_YL_THSY_t;

/*===============================================================================================*/

/*
=====================================================================================================
	IG_THS_ZH: | - | THSZ14 | THSZ13 | THSZ12 | THSZ11 | THSZ10 | THSZ9 | THSZ8 |
=====================================================================================================
*/

/* Interrupt threshold on Z axis. Default value: 000 0000 */
typedef enum IG_THS_ZH_THSZ_enum
{
	IG_THS_ZH_THSZ_8_bm				= (0x01<<0),			
	IG_THS_ZH_THSZ_9_bm				= (0x01<<1),			
	IG_THS_ZH_THSZ_10_bm			= (0x01<<2),			
	IG_THS_ZH_THSZ_11_bm			= (0x01<<3),			
	IG_THS_ZH_THSZ_12_bm			= (0x01<<4),			
	IG_THS_ZH_THSZ_13_bm			= (0x01<<5),			
	IG_THS_ZH_THSZ_14_bm			= (0x01<<6)				
	
} IG_THS_ZH_THSZ_t;


/*===============================================================================================*/

/*
=====================================================================================================
	IG_THS_ZL: | THSZ7 | THSZ6 | THSZ5 | THSZ4 | THSZ3 | THSZ2 | THSZ1 | THSZ0 |
=====================================================================================================
*/

/* Interrupt threshold on Z axis. Default value: 0000 0000 */
typedef enum IG_THS_ZL_THSZ_enum
{
	IG_THS_ZL_THSZ_0_bm				= (0x01<<0),			
	IG_THS_ZL_THSZ_1_bm				= (0x01<<1),			
	IG_THS_ZL_THSZ_2_bm				= (0x01<<2),			
	IG_THS_ZL_THSZ_3_bm				= (0x01<<3),			
	IG_THS_ZL_THSZ_4_bm				= (0x01<<4),			
	IG_THS_ZL_THSZ_5_bm				= (0x01<<5),			
	IG_THS_ZL_THSZ_6_bm				= (0x01<<6),
	IG_THS_ZL_THSZ_7_bm				= (0x01<<7)				
	
} IG_THS_ZL_THSZ_t;

/*===============================================================================================*/

/*
=====================================================================================================
	IG_DURATION: | WAIT | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
=====================================================================================================
*/

/* Interrupt generation counter mode selection. Default value: 0 */
#define IG_DURATION_WAIT_0_bm		(0x00<<7)			/*  disable */
#define IG_DURATION_WAIT_1_bm		(0x01<<7)			/*  enable */

/* 
	Default value: 000 0000
	D6 - D0 bits set the minimum duration of the Interrupt event to be recognized. 
	Duration steps and maximum values depend on the ODR chosen.
*/
typedef enum IG_DURATION_D_enum
{
	IG_DURATION_D0_bm				= (0x01<<0),			
	IG_DURATION_D1_bm				= (0x01<<1),			
	IG_DURATION_D2_bm				= (0x01<<2),			
	IG_DURATION_D3_bm				= (0x01<<3),			
	IG_DURATION_D4_bm				= (0x01<<4),			
	IG_DURATION_D5_bm				= (0x01<<5),			
	IG_DURATION_D6_bm				= (0x01<<6)				
	
} IG_DURATION_D_t;


/*===============================================================================================*/


/*
=====================================================================================================
	LOW_ODR: | -- | -- | DRDY_HL | 0 | I2C_dis | SW_RES | 0 | Low_ODR |
=====================================================================================================
*/

/* DRDY/INT2 pin active level. Default value: 0 */
#define LOW_ODR_DRDY_HL_0_bm		(0x00<<5)			/*  DRDY active high */
#define LOW_ODR_DRDY_HL_1_bm		(0x01<<5)			/*  DRDY active low */


#define LOW_ODR_I2C_dis_0_bm		(0x00<<3)			/*  both the I2C and SPI interfaces enabled (default) */
#define LOW_ODR_I2C_dis_1_bm		(0x01<<3)			/*  SPI only */

/* Software reset. Default value: 0 */
#define LOW_ODR_SW_RES_0_bm			(0x00<<2)			/*  Normal Mode */
#define LOW_ODR_SW_RES_1_bm			(0x01<<2)			/*  Reset Device (this bit is cleared by hardware after next flash boot) */

/* Low speed ODR. Default value: 0 */
#define LOW_ODR_Low_ODR_0_bm		(0x00<<0)			/*  Low Speed ODR disabled */
#define LOW_ODR_Low_ODR_1_bm		(0x01<<0)			/*  Low Speed ODR enabled */

/*===============================================================================================*/


#endif
