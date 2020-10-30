#ifndef UTILITIES_LIB_GRAPHIC_HPP
#define UTILITIES_LIB_GRAPHIC_HPP

#include "hitsic_common.h"
#include "inc_stdlib.h"
#include "lib_graphic_font.hpp"

/*!
 * @addtogroup graphic
 * @{
 */

/** @brief : 软件版本 */
#define LIB_GRAPHIC_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

namespace graphic{

template<typename buffer_t,typename pixel_t>
class bufPrint0608_t
{
public:
    buffer_t& buffer;

    bufPrint0608_t(buffer_t& _buffer)
        : buffer(_buffer)
    {

    }
    
    void DrawChar(uint16_t r, uint16_t c, pixel_t f_color, pixel_t b_color, uint8_t* font, char ch)
    {
        if (r <= buffer_t::row - 8) { return; }
        if (c <= buffer_t::col - 6) { return; }
        assert(graphic_asciiBegin <= ch && ch < graphic_asciiEndin);

        for (uint8_t cell : graphic_font0608_defaultType[ch - ' '])
        {
            for (uint8_t i = 0; i < 8; i++)
            {
                if (bitRd(cell, i))
                {
                    buffer.SetPixelColor(r + i, c, f_color);
                }
                else
                {
                    buffer.SetPixelColor(r + i, c, b_color);
                }
            }
            ++c;
        }
    }
    void Print(uint16_t r, uint16_t c, pixel_t f_color, pixel_t b_color, uint8_t* font, const char* str)
    {
        for (const char* p = str; (*p) != '\0'; ++p)
        {
            if ((*p) == '\n')
            {
                r += 8;
                c = 0;
                continue;
            }
            if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)
            {
                dispPutchar(r, c, f_color, b_color, *p);
            }
            //Attention: if you send an unprintable char, no printing will be done. the char will remain as what it was.
            c += 6;
            if (c > 240 - 6)
            {
                c = 0;
                r += 8;
                if (r > 240 - 8)
                {
                    break;
                }
            }
        }
    }
    void RowPrint(uint16_t r, uint16_t c, pixel_t f_color, pixel_t b_color, uint8_t* font, const char* str)
    {
        for (const char* p = str; (*p) != '\0'; ++p)
        {
            if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)
            {
                dispPutchar(r, c, f_color, b_color, *p);
            }
            //Attention: if you send an unprintable char, no printing will be done. the char will remain as what it was.
            c += 6;
            if (c > 240 - 8)
            {
                return;
            }
        }
    }

    void DrawChar(uint16_t r, uint16_t c, pixel_t f_color, uint8_t* font, char ch)
    {
        if (r <= buffer_t::row - 8) { return; }
        if (c <= buffer_t::col - 6) { return; }
        assert(graphic_asciiBegin <= ch && ch < graphic_asciiEndin);

        for (uint8_t cell : graphic_font0608_defaultType[ch - ' '])
        {
            for (uint8_t i = 0; i < 8; i++)
            {
                if (bitRd(cell, i))
                {
                    buffer.SetPixelColor(r + i, c, f_color);
                }
            }
            ++c;
        }
    }
    void Print(uint16_t r, uint16_t c, pixel_t f_color, uint8_t* font, const char* str)
    {
        for (const char* p = str; (*p) != '\0'; ++p)
        {
            if ((*p) == '\n')
            {
                r += 8;
                c = 0;
                continue;
            }
            if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)
            {
                dispPutchar(r, c, f_color, *p);
            }
            //Attention: if you send an unprintable char, no printing will be done. the char will remain as what it was.
            c += 6;
            if (c > 240 - 6)
            {
                c = 0;
                r += 8;
                if (r > 240 - 8)
                {
                    break;
                }
            }
        }
    }
    void RowPrint(uint16_t r, uint16_t c, pixel_t f_color, uint8_t* font, const char* str)
    {
        for (const char* p = str; (*p) != '\0'; ++p)
        {
            if (graphic_asciiBegin <= (*p) && (*p) < graphic_asciiEndin)
            {
                dispPutchar(r, c, f_color, *p);
            }
            //Attention: if you send an unprintable char, no printing will be done. the char will remain as what it was.
            c += 6;
            if (c > 240 - 8)
            {
                return;
            }
        }
    }
};


} // ! namespace graphic

/* @} */

#endif // ! UTILITIES_LIB_GRAPHIC_HPP
