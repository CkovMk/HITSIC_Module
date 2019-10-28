#ifndef _SMARTCAR_UPLOAD_H
#define _SMARTCAR_UPLOAD_H
#include "stdafx.h"
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

#ifdef __cplusplus
extern "C" {
#endif

void Send_Begin(void);
void  Send_Variable(void);
void Img_Upload(void);
void Img_Upload_wxj(uint8_t* upload_img);


#ifdef __cplusplus
}
#endif


#endif
