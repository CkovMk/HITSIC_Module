#ifndef UTILITIES_LIB_GRAPHIC_HPP
#define UTILITIES_LIB_GRAPHIC_HPP

#include "hitsic_common.h"

namespace graphic{

template<typename pixel_t, const int16_t row, const int16_t col>
class frameBuffer_t
{
public:
    pixel_t pixel[row][col];
};

}



#endif // ! UTILITIES_LIB_GRAPHIC_HPP
