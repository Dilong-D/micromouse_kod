#define ERR_SUM_MAX		1000
#define PD_OUTPUT_MAX 100
#include "libs.h"
#include "pd.h"
#include "pid.h"
#include "pos_dir_enc.h"




void pd_init(float kr, float Td)
{
	pd_params.kr = kr;
	pd_params.Td = Td;
	pd_params.err = 0;
	pd_params.err_last = 0;
}


float pd_calculate(float set_val, float read_val, struct pd_params * pd_params1)
{
	
	float p,d,u;
	
	pd_params1->err = set_val - read_val;
	
	p =  pd_params1->kr * pd_params1->err;
	d =  pd_params1->kr * pd_params1->Td * (pd_params1->err - pd_params1->err_last) / t_int;
	
	u = p+d;
	
	pd_params1->err_last = pd_params1->err;
	return u;
}


void rotate(float ang){
	

	
	float angle = par.dir; 
	float u;
	u = pd_calculate(ang, angle,&pd_params);	
	wheel(u,-u);
	
	LcdClear();
	Lcd("angle");
	LcdDec(par.dir);
	_delay_ms(10);
}
