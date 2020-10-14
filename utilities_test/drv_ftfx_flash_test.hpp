/**
 * Copyright 2018 - 2010 HITSIC
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

/**
 * @file 	:	drv_ftfx_flash_test.hpp
 * @author  :	Jerry Skywalker
 * @version :	v0.1。0
 *
 * @date 	:	v0.1.0 2020.10.8
 *
 * @note    :   依赖库：drv_ftfx_flash、drv_button、drv_disp_ssd1306
 依赖库必须先初始化。
 * @note 	:	预发布版本，代码不完整，仅供学习。
 */

#include "drv_ftfx_flash.h"

status_t drv_ftfx_flash_UnitTest(void)
{
	if(FLASH_SimpleInit())
	{
		return kStatus_Fail;
	}



	return kStatus_Success;

}

