/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 *	@by:		SmartCar SJMC
 *	@name:		drvoled.h
 *	@layer:		driver
 *	@owner:		C.M.@SJMC
 *	@modified:	C.M.@SJMC 2019.01.15 04:48
 */
#pragma once
#ifndef D_DEFAULT_DRVOLED_H_
#define D_DEFAULT_DRVOLED_H_
#include <inc_stdlib.h>
#include "hitsic_common.h"
#include "sys_pitmgr.hpp"
#include "pin_mux.h"

#define HITSIC_DISP_SSD1306_FLIP_X (0U)	///< ��Ļ���ҷ�ת
#define HITSIC_DISP_SSD1306_FLIP_Y (0U)	///< ��Ļ���·�ת


/**
 * @brief ����RST�ŵ�ƽ��
 * 
 * @param x RST�ŵĵ�ƽ״̬�����߼���
 */
inline void DISP_SSD1306_gpioSetRST(uint8_t x)
{

}

/**
 * @brief ����D/C�ŵ�ƽ��
 * 
 * @param x D/C�ŵĵ�ƽ״̬�����߼���
 */
inline void DISP_SSD1306_gpioSetD_C(uint8_t x)
{

}

/**
 * @brief ���뼶�����ӳٺ�����
 * 
 * @param ms �ӳٵĺ�������
 */
inline void DISP_SSD1306_delay_ms(uint32_t ms)
{

}


/**
 * @brief SPI�ӿڷ���һ���ֽڡ�
 * 
 * @param data Ҫ���͵�����
 */
inline void DISP_SSD1306_spiWrite(uint8_t data)
{

}






#endif // ! D_DEFAULT_DRVOLED_H_
