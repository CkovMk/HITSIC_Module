#ifndef _LIB_ADRCCTRL_H_
#define _LIB_ADRCCTRL_H_


#include "inc_stdlib.h"
#include "hitsic_common.h"

#ifdef __cplusplus
extern "C"{
#endif


typedef struct
{
	float v1;
	float v2;
	float r2;
	float h2;
	float h;
}adrc_tdCtrl_t;

typedef struct
{
	float v1;
	float v2;
	float r0;
	float h0;
	float h;
}adrc_td_t;

typedef struct
{
	float h;
	float beta1;
	float beta2;
	float alpha;
	float delta;
	float u;
	float b0;
	/* ESO */
	float z1;
	float z2;
}adrc_eso_t;

typedef struct
{
	float h;
	float beta1;
	float beta2;
	float u;
	float b0;
	/* LESO */
	float z1;
	float z2;
}adrc_leso_t;	/* Linear ESO */

typedef struct
{
	float h;
	float h1;
	float r1;
	float c;
}adrc_nlsef_t;

float ADRCCTRL_Sign(float val);

void ADRCCTRL_TDInit(adrc_td_t* td_t, float h, float r0, float h0);
void ADRCCTRL_TD(adrc_td_t* td, float v);
void ADRCCTRL_TdCtrlInit(adrc_tdCtrl_t* td_controller, float h, float r2, float h2);
float ADRCCTRL_TdCtrl(adrc_tdCtrl_t* td_controller, float err);
void ADRCCTRL_EsoInit(adrc_eso_t* eso_t, float h, float beta1, float beta2, float alpha, float delta, float b0);
void ADRCCTRL_Eso(adrc_eso_t* eso_t, float y);
void ADRCCTRL_LesoInit(adrc_leso_t* leso_t, float h, float w, float b0);
void ADRCCTRL_Leso(adrc_leso_t* leso_t, float y);
void ADRCCTRL_NlsefInit(adrc_nlsef_t* nlsef_t, float h, float r1, float h1, float c);
float ADRCCTRL_Nlsef(adrc_nlsef_t* nlsef_t, float e1, float e2);









#ifdef __cplusplus
}
#endif



#endif // ! _LIB_ADRCCTRL_H_


