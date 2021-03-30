#ifndef UTILITIES_LIB_GRAPHIC_HPP
#define UTILITIES_LIB_GRAPHIC_HPP

#include <inc_stdlib.h>
#include "hitsic_common.h"

#include <lib_graphic_font.h>
/*!
 * @addtogroup graphic
 * @{
 */

/** @brief : 软件版本 */
#define LIB_GRAPHIC_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))


#define GRAPHIC_FB_MONO_DEF(var_name, fun_name, row, col)                               \
  typedef bool var_name##_fb_pixel_t;                                                   \
  const uint16_t var_name##_fb_row = row;                                               \
  const uint16_t var_name##_fb_col = col;                                               \
  typedef struct _##var_name##_fb                                                       \
  {                                                                                     \
      uint8_t frame[row >> 3][col];                                                     \
  } var_name##_fb_t;                                                                    \
  static inline void fun_name##_FB_Clear(var_name##_fb_t *inst)                         \
  {                                                                                     \
      memset(inst, 0U, sizeof(var_name##_fb_t));                                        \
  }                                                                                     \
  static inline void fun_name##_FB_SetPixelColor                                        \
    (var_name##_fb_t *inst, uint16_t x, uint16_t y, var_name##_fb_pixel_t color)        \
  {                                                                                     \
      color ? (inst->frame[y >> 3][x] |= (1U << (y & 7U)))                              \
          : (inst->frame[y >> 3][x] &= ~(1U << (y & 7U)));                              \
  }





#define GRAPHIC_PRINT0608_DEF(var_name, fun_name)                                       \
  static inline void fun_name##_Print0608_DrawChar_Overlay                              \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, var_name##_fb_pixel_t b_color, char ch) \
  {                                                                                     \
      if (r > var_name##_fb_row - 8) { return; }                                        \
      if (c > var_name##_fb_col - 6) { return; }                                        \
      assert(graphic_asciiBegin <= ch && ch < graphic_asciiEndin);                      \
      for (uint8_t cell : graphic_font0608_defaultType[ch - ' '])                       \
      {                                                                                 \
          for (uint8_t i = 0; i < 8; i++)                                               \
          {                                                                             \
              if (bitRd(cell, i))                                                       \
              {                                                                         \
            	  fun_name##_FB_SetPixelColor(inst, c, r + i, f_color);                 \
              }                                                                         \
              else                                                                      \
              {                                                                         \
            	  fun_name##_FB_SetPixelColor(inst, c, r + i, b_color);                 \
              }                                                                         \
          }                                                                             \
          ++c;                                                                          \
      }                                                                                 \
  }                                                                                     \
  static inline void fun_name##_Print0608_Print_Overlay                                 \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, var_name##_fb_pixel_t b_color, const char* str) \
  {                                                                                     \
      for (const char* p = str; (*p) != '\0'; ++p)                                      \
      {                                                                                 \
          if ((*p) == '\n')                                                             \
          {                                                                             \
              r += 8;                                                                   \
              c = 0;                                                                    \
              continue;                                                                 \
          }                                                                             \
          if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)                  \
          {                                                                             \
              fun_name##_Print0608_DrawChar_Overlay(inst, r, c, f_color, b_color, *p);  \
          }                                                                             \
          c += 6;                                                                       \
          if (c > 240 - 6)                                                              \
          {                                                                             \
              c = 0;                                                                    \
              r += 8;                                                                   \
              if (r > 240 - 8)                                                          \
              {                                                                         \
                  break;                                                                \
              }                                                                         \
          }                                                                             \
      }                                                                                 \
  }                                                                                     \
  static inline void fun_name##_Print0608_RowPrint_Overlay                              \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, var_name##_fb_pixel_t b_color, const char* str) \
  {                                                                                     \
      for (const char* p = str; (*p) != '\0'; ++p)                                      \
      {                                                                                 \
          if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)                  \
          {                                                                             \
              fun_name##_Print0608_DrawChar_Overlay(inst, r, c, f_color, b_color, *p);  \
          }                                                                             \
          c += 6;                                                                       \
          if (c > 240 - 8)                                                              \
          {                                                                             \
              return;                                                                   \
          }                                                                             \
      }                                                                                 \
  }                                                                                     \
  static inline void fun_name##_Print0608_DrawChar_Hyaline                              \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, char ch) \
  {                                                                                     \
      if (r > var_name##_fb_row - 8) { return; }                                        \
      if (c > var_name##_fb_col - 6) { return; }                                        \
      assert(graphic_asciiBegin <= ch && ch < graphic_asciiEndin);                      \
      for (uint8_t cell : graphic_font0608_defaultType[ch - ' '])                       \
      {                                                                                 \
          for (uint8_t i = 0; i < 8; i++)                                               \
          {                                                                             \
              if (bitRd(cell, i))                                                       \
              {                                                                         \
                  fun_name##_FB_SetPixelColor(inst, c, r + i, f_color);                 \
              }                                                                         \
          }                                                                             \
          ++c;                                                                          \
      }                                                                                 \
  }                                                                                     \
  static inline void fun_name##_Print0608_Print_Hyaline                                 \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, const char* str) \
  {                                                                                     \
      for (const char* p = str; (*p) != '\0'; ++p)                                      \
      {                                                                                 \
          if ((*p) == '\n')                                                             \
          {                                                                             \
              r += 8;                                                                   \
              c = 0;                                                                    \
              continue;                                                                 \
          }                                                                             \
          if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)                  \
          {                                                                             \
              fun_name##_Print0608_DrawChar_Hyaline(inst, r, c, f_color, *p);           \
          }                                                                             \
          c += 6;                                                                       \
          if (c > 240 - 6)                                                              \
          {                                                                             \
              c = 0;                                                                    \
              r += 8;                                                                   \
              if (r > 240 - 8)                                                          \
              {                                                                         \
                  break;                                                                \
              }                                                                         \
          }                                                                             \
      }                                                                                 \
  }                                                                                     \
  static inline void fun_name##_Print0608_RowPrint_Hyaline                              \
    (var_name##_fb_t *inst, uint16_t r, uint16_t c, var_name##_fb_pixel_t f_color, const char* str) \
  {                                                                                     \
      for (const char* p = str; (*p) != '\0'; ++p)                                      \
      {                                                                                 \
          if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)                  \
          {                                                                             \
              fun_name##_Print0608_DrawChar_Hyaline(inst, r, c, f_color, *p);           \
          }                                                                             \
          c += 6;                                                                       \
          if (c > 240 - 8)                                                              \
          {                                                                             \
              return;                                                                   \
          }                                                                             \
      }                                                                                 \
  }


//TODO: for (uint8_t cell : graphic_font0608_defaultType[ch - ' '])




/* @} */

#endif // ! UTILITIES_LIB_GRAPHIC_HPP
