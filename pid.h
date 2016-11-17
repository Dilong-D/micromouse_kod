#ifndef PID_H_
#define PID_H_

//struct nowa
//{
//float u;
//unsigned int params;
//};

struct pid_params
{
	float kr;
	float Ti;
	float Td;
	float err;
	float err_sum;
	float err_last;
};

static struct pid_params pid_params;
static struct pid_params pid_params2;

void pid_init(float kp, float ki, float kd,float kp_l, float ki_l, float kd_l);

float pid_calculate(float set_val, float read_val,struct pid_params * pid_params1);

void l_wheel(float v);
void r_wheel(float v);
void wheel(float vl, float vr);

int itab;
float tab[1000];
#endif /* PID_H_ */
