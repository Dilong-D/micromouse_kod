

#define ERR_SUM_MAX		1000
#define PID_OUTPUT_MAX 100
#include "libs.h"
#include "pid.h"



//struct nowa
//{
//float u;
//unsigned int params;
//};


void pid_init(float kp, float ki, float kd, float kp_l, float ki_l, float kd_l)
{
	pid_params.kp = kp;
	pid_params.ki = ki;
	pid_params.kd = kd;
	pid_params.err = 0;
	pid_params.err_sum = 0;
	pid_params.err_last = 0;
	
	pid_params2.kp = kp_l;
	pid_params2.ki = ki_l;
	pid_params2.kd = kd_l;
	pid_params2.err = 0;
	pid_params2.err_sum = 0;
	pid_params2.err_last = 0;
}

float pid_calculate(float set_val, float read_val, struct pid_params * pid_params1)
{
	if(read_val > 30000)
	read_val = read_val - 65536;
	float err_d, u;
	
	pid_params1->err = set_val - read_val;
	err_d = pid_params1->err_last - pid_params1->err;
	pid_params1->err_sum += pid_params1->err;
	u = pid_params1->kp * pid_params1->err + pid_params1->ki * pid_params1->err_sum + pid_params1->kd * err_d;
	if((u > PID_OUTPUT_MAX) || (u <  -PID_OUTPUT_MAX)){
		pid_params1->err_sum -= pid_params1->err;
		u = pid_params1->kp * pid_params1->err + pid_params1->ki * pid_params1->err_sum
		+ pid_params1->kd * err_d;
	}

	//
	//moja.u=u;
	//moja.params = (unsigned int)pid_params.err_sum;
	return u;
}

void l_wheel(float v)
{
	float u;
	u = pid_calculate(v,L_ENKODER,&pid_params);
	
	if(u > 0)
	runR(u,LEWO);
	
	else if(u < 0)
	runR(-u,PRAWO);
	
	else if(u  == 0)
	runR(0,STOP);
	
	L_ENKODER=0;
}

void r_wheel(float v)
{
	float u;
	u = pid_calculate(v,R_ENKODER,&pid_params2);
	
	if(u > 0)
	runL(u,PRAWO);
	
	else if(u < 0)
	runL(-u,LEWO);
	
	else if(u == 0)
	runL(0,STOP);
	
	R_ENKODER=0;
}