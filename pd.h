#ifndef PD_H_
#define PD_H_

struct pd_params
{
	float kr;
	//float Ti;
	float Td;
	float err;
	float err_sum;
	float err_last;
};

static struct pd_params pd_params;


void pd_init(float kp,float kd);

float pd_calculate(float set_val, float read_val, struct pd_params * pd_params1);
void rotate(float);



#endif /* PD_H_ */
