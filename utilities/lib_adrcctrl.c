#include <lib_adrcctrl.h>

#ifdef __cplusplus
extern "C"{
#endif


float ADRCCTRL_Sign(float val)
{
	if(val >= 0.0f)
		return 1.0f;
	else
		return -1.0f;
}

float ADRCCTRL_Fhan(float v1, float v2, float r0, float h0)
{
	float d = h0 * h0 * r0;
	float a0 = h0 * v2;
	float y = v1 + a0;
	float a1 = sqrtf(d*(d + 8.0f*fabsf(y)));
	float a2 = a0 + ADRCCTRL_Sign(y)*(a1-d)*0.5f;
	float sy = (ADRCCTRL_Sign(y+d) - ADRCCTRL_Sign(y-d))*0.5f;
	float a = (a0 + y - a2)*sy + a2;
	float sa = (ADRCCTRL_Sign(a+d) - ADRCCTRL_Sign(a-d))*0.5f;
	
	return -r0*(a/d - ADRCCTRL_Sign(a))*sa - r0*ADRCCTRL_Sign(a);
}

//原点附近有连线性段的连续幂次函数
float ADRCCTRL_Fal(float e, float alpha, float delta)
{
	if(fabsf(e) <= delta){
		return e / (powf(delta, 1.0f-alpha));
	}else{
		return powf(fabsf(e), alpha) * ADRCCTRL_Sign(e);
	}
}

//初始化，传入参数仅进行拷贝
void ADRCCTRL_TDInit(adrc_td_t *td_t, float h)
{
	memset(td_t, 0, sizeof(adrc_td_t));
	td_t->h = h;
	td_t->r = FLT_MAX;
}

//更新一次TD
void ADRCCTRL_TD(adrc_td_t *td, float v)
{
	float fv = ADRCCTRL_Fhan(td->v1 - v, td->v2, td->r, td->h);
	
	td->v1 += td->h * td->v2;
	td->v2 += td->h * fv;
}

//初始化，传入参数仅进行拷贝
void ADRCCTRL_Init(adrc_t *p,float h)
{
	memset(p, 0, sizeof(adrc_t));
	p->h = h;
	p->r = FLT_MAX;
	p->u_max = 0;
	p->u_min = 0;
	p->b = 1;
	p->w0 = 1;
	p->kp = 1;
	p->kd = 1;

}


//更新一次adrc控制
void ADRCCTRL_Update(adrc_t *p)
{
	//先计算td
	float fh = ADRCCTRL_Fhan(p->v1 - p->v, p->v2, p->r, p->h);
	p->v1 += p->h * p->v2;
	p->v2 += p->h * fh;

	//计算控制量
	p->b01 = 3 * p->w0;
	p->b02 = 3 * p->w0 * p->w0;
	p->b03 = p->w0 * p->w0 * p->w0;

	p->e = p->z1 - p->y;
	//更新中间量
	p->z1 = p->z1 + p->h * (p->z2 - p->b01 * p->e);
	p->z2 = p->z2 + p->h * (p->z3 - p->b02 * p->e + p->b * p->u);
	p->z3 = p->z3 - p->h * p->b03 * p->e;

	p->e1 = p->v1 - p->z1;
	p->e2 = p->v2 - p->z2;
	//p->e1 = p->v - p->z1;
	//p->e2 = 0 - p->z2;
	p->u0 = p->kp * p->e1 + p->kd * p->e2;
	p->u = p->u0 - p->z3 / p->b;

	if (p->u > p->u_max)
	{
		p->u = p->u_max;
	}
	else if (p->u < p->u_min)
	{
		p->u = p->u_min;
	}
}

//更新一次adrc控制，只不过是非线性d
void ADRCCTRL_UpdateFal(adrc_t *p, float d_line)
{
	//先计算td
	p->v1 += p->h * p->v2;
	p->v2 += p->h * ADRCCTRL_Fhan(p->v1 - p->v, p->v2, p->r, p->h);

	//计算控制量
	float w0_2 = p->w0 * p->w0;
	float w0_3 = w0_2 * p->w0;
	p->b01 = 3 * p->w0;
	p->b02 = 3 * w0_2;
	p->b03 = w0_3;

	p->e = p->z1 - p->y;
	//更新中间量
	p->z1 = p->z1 + p->h * (p->z2 - p->b01 * p->e);
	p->z2 = p->z2 + p->h * (p->z3 - p->b02 * p->e + p->b * p->u);
	p->z3 = p->z3 - p->h * p->b03 * p->e;

	p->e1 = p->v1 - p->z1;
	p->e2 = p->v2 - p->z2;

	p->u0 = p->kp * p->e1 + p->kd * ADRCCTRL_Fal(p->e2, 0.25, d_line);
	p->u = p->u0 - p->z3 / p->b;

	if (p->u > p->u_max)
	{
		p->u = p->u_max;
	}
	else if (p->u < p->u_min)
	{
		p->u = p->u_min;
	}
}


#ifdef __cplusplus
}
#endif




