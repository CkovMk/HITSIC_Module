/**
 * Copyright 2018 - 2021 HITSIC
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
 * @file    :   app_svbmp.hpp
 * @author  :   beforelight/肖日涛
 * @version :   v0.1.0
 *
 * @date    :   v0.1.0      2020.08.04
 *
 */

#ifndef UTILITIES_APP_SVBMP_HPP
#define UTILITIES_APP_SVBMP_HPP
#include <inc_stdlib.h>
#include "hitsic_common.h"

#if (defined(HITSIC_USE_APP_SVBMP) && (HITSIC_USE_APP_SVBMP > 0))
#include <app_svbmp_port.h>

 /*!
  * @addtogroup svbmp
  * @{
  */

/** @brief : 软件版本 */
#define APP_SVBMP_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

typedef enum _camera_pixel_format
{
    PixelFormatGray = 1,//灰度
    PixelFormatRGB565 = 2,
}svbmp_pixelFormat_t;


typedef struct __img {
    svbmp_pixelFormat_t format;
    uint16_t height;
    uint16_t width;
    void* pImg;
}svbmp_img_t;

int SVBMP_Save(svbmp_file_t* fp, svbmp_img_t* img);

/* @} */

#endif // ! HITSIC_USE_APP_SVBMP

#endif // ! UTILITIES_APP_SVBMP_HPP
