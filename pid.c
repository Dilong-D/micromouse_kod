

#define ERR_SUM_MAX		1000
#define PID_OUTPUT_MAX 100
#include "libs.h"
#include "pid.h"



uint16_t cnt = 0;
itab = 0;


void pid_init(float kr, float Ti, float Td, float kr_l, float Ti_l, float Td_l)
{
	pid_params.kr = kr;
	pid_params.Ti = Ti;
	pid_params.Td = Td;
	pid_params.err = 0;
	pid_params.err_sum = 0;
	pid_params.err_last = 0;
	
	pid_params2.kr = kr_l;
	pid_params2.Ti = Ti_l;
	pid_params2.Td = Td_l;
	pid_params2.err = 0;
	pid_params2.err_sum = 0;
	pid_params2.err_last = 0;
}

uint8_t t = 0;

float pid_calculate(float set_val, float read_val, struct pid_params * pid_params1)
{
	if(read_val > 30000)
	read_val = read_val - 65536;
	float p,i,d, u;
	
	pid_params1->err = set_val - read_val;
	pid_params1->err_sum += pid_params1->err;
	
	p =  pid_params1->kr * pid_params1->err;
	i =  pid_params1->kr * t_int * pid_params1->err_sum /pid_params1->Ti;
	d =  pid_params1->kr * pid_params1->Td * (pid_params1->err - pid_params1->err_last) / t_int;
	
	u = p+i+d;
	if((u > PID_OUTPUT_MAX) || (u <  -PID_OUTPUT_MAX)){
		pid_params1->err_sum -= pid_params1->err;
		i =  pid_params1->kr * t_int * pid_params1->err_sum /pid_params1->Ti;
		u = p+i+d;
	}
	
	
	pid_params1->err_last = pid_params1->err;
	return u;


}


void wheel(float vl, float vr){
	uint16_t enkl = L_ENKODER;
	L_ENKODER=0;
	uint16_t enkr = R_ENKODER;
	R_ENKODER=0;
	old_enk_l = enkl;
	old_enk_r = enkr;
	
	

	
	
	float ul,ur;
	float l,r;
	
	vl = - vl;
	
	//l = 2*L_ENKODER - R_ENKODER;
	if(vl == 0){
		
		//ul = 0;
		//TCD0.CCA		=	100;
		//PORTD.OUTSET	=	PIN1_bm;//input 1->1
		//PORTD.OUTSET	=	PIN2_bm;//input 2->1
		pid_params2.err_sum = 0;
		pid_params2.err_last = 0;
	}
	else{
		ul = pid_calculate(vl, enkl,&pid_params2);
	}
	
	//r = 2*R_ENKODER - L_ENKODER;
	if(vr == 0){
		
// 		ur = 0;
// 		TCD0.CCD		=	100;
// 		PORTD.OUTSET	=	PIN5_bm;//input 1->1
// 		PORTD.OUTSET	=	PIN4_bm;//input 2->1
		pid_params.err_sum = 0;
		pid_params.err_last = 0;
	}
	else{
		ur = pid_calculate(vr, enkr,&pid_params);
	}
	//if(vr != 0){
		if(ul >= 0)
			runL(ul,LEWO);
	
		else 
			runL(-ul,PRAWO);
	
	
	if(ur >= 0)
		runR(ur,PRAWO);
	
	else 
		runR(-ur,LEWO);
	
	
	
}
