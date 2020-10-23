#ifndef UTILITIES_LIB_GRAPHIC_HPP
#define UTILITIES_LIB_GRAPHIC_HPP

#include "hitsic_common.h"

#include "lib_graphic_font.hpp"

namespace graphic{

template<typename buffer_t,typename pixel_t>
class bufPrint0608_t
{
public:
    buffer_t& buffer;

    bufferPrint_t(buffer_t& _buffer)
        : buffer(_buffer)
    {

    }
    
    void DrawChar(uint16_t x, uint16_t y, pixel_t f_color, pixel_t b_color, uint8_t* font, char ch)
    {
        
    }
    void PrintStr(uint16_t x, uint16_t y, pixel_t f_color, pixel_t b_color, uint8_t* font, const char* str);
    void PrintString(uint16_t x, uint16_t y, pixel_t f_color, pixel_t b_color, uint8_t* font, std::string& str);
    void RowPrintStr(uint16_t x, uint16_t y, pixel_t f_color, pixel_t b_color, uint8_t* font, const char* str);
    void RowPrintString(uint16_t x, uint16_t y, pixel_t f_color, pixel_t b_color, uint8_t* font, std::string& str);

    void DrawChar(uint16_t x, uint16_t y, pixel_t f_color, uint8_t* font, char ch);
    void PrintStr(uint16_t x, uint16_t y, pixel_t f_color, uint8_t* font, const char* str);
    void PrintString(uint16_t x, uint16_t y, pixel_t f_color, uint8_t* font, std::string& str);
    void RowPrintStr(uint16_t x, uint16_t y, pixel_t f_color, uint8_t* font, const char* str);
    void RowPrintString(uint16_t x, uint16_t y, pixel_t f_color, uint8_t* font, std::string& str);
};


} // ! namespace graphic



#endif // ! UTILITIES_LIB_GRAPHIC_HPP
