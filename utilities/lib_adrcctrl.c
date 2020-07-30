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

void ADRCCTRL_TDInit(adrc_td_t* td_t, float h, float r0, float h0)
{
	td_t->h = h;
	td_t->r0 = r0;
	td_t->h0 = h0;
	td_t->v1 = td_t->v2 = 0.0f;
}

void ADRCCTRL_TD(adrc_td_t* td, float v)
{
	float fv = ADRCCTRL_Fhan(td->v1 - v, td->v2, td->r0, td->h0);
	
	td->v1 += td->h * td->v2;
	td->v2 += td->h * fv;
}

void ADRCCTRL_TdCtrlInit(adrc_tdCtrl_t* td_controller, float h, float r2, float h2)
{
	td_controller->v1 = 0.0f;
	td_controller->v2 = 0.0f;
	td_controller->r2 = r2;
	td_controller->h2 = h2;
	td_controller->h = h;
}

float ADRCCTRL_TdCtrl(adrc_tdCtrl_t* td_controller, float err)
{
	float fv = ADRCCTRL_Fhan(-err, td_controller->v2, td_controller->r2, td_controller->h2);
	td_controller->v1 += td_controller->h * td_controller->v2;
	td_controller->v2 += td_controller->h * fv;
	
	return td_controller->v2;
}

void ADRCCTRL_EsoInit(adrc_eso_t* eso_t, float h, float beta1, float beta2, float alpha, float delta, float b0)
{
	eso_t->h = h;
	eso_t->beta1 = beta1;
	eso_t->beta2 = beta2;
	eso_t->u = 0.0f;
	eso_t->alpha = alpha;
	eso_t->delta = delta;
	eso_t->b0 = b0;
	
	eso_t->z1 = eso_t->z2 = 0.0f;
}

void ADRCCTRL_Eso(adrc_eso_t* eso_t, float y)
{
	float e = eso_t->z1 - y;
	float fe = ADRCCTRL_Fal(e, eso_t->alpha, eso_t->delta);
	
	eso_t->z1 += eso_t->h*(eso_t->z2 + eso_t->b0*eso_t->u - eso_t->beta1*e);
    eso_t->z2 -= eso_t->h*eso_t->beta2*fe;
}

void ADRCCTRL_LesoInit(adrc_leso_t* leso_t, float h, float w, float b0)
{
	leso_t->h = h;
	// (s + w)^2 = s^2 + beta_1 * s + beta_2
	leso_t->beta1 = 2.0f*w;
	leso_t->beta2 = w*w;
	leso_t->u = 0.0f;
	leso_t->b0 = b0;
	
	leso_t->z1 = leso_t->z2 = 0.0f;
}

void ADRCCTRL_Leso(adrc_leso_t* leso_t, float y)
{
	float e = leso_t->z1 - y;
	
	leso_t->z1 += leso_t->h*(leso_t->z2 + leso_t->b0*leso_t->u - leso_t->beta1*e);
    leso_t->z2 -= leso_t->h*leso_t->beta2*e;
}

void ADRCCTRL_NlsefInit(adrc_nlsef_t* nlsef_t, float h, float r1, float h1, float c)
{
	nlsef_t->h = h;
	nlsef_t->h1 = h1;
	nlsef_t->r1 = r1;
	nlsef_t->c = c;
}

float ADRCCTRL_Nlsef(adrc_nlsef_t* nlsef_t, float e1, float e2)
{
	return -ADRCCTRL_Fhan(e1, nlsef_t->c*e2, nlsef_t->r1, nlsef_t->h1);
}






#ifdef __cplusplus
}
#endif




