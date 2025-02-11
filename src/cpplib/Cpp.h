#ifndef CPP_H
#define CPP_H
#include "Global.h"


/*!
 * This contains the C++ language implementation.
 */
namespace Cpp
{
    class Language;


    /*!
     * This enumerates all source code versions for C++ parsing and building.
     */
    enum Version
    {
        Cpp_Legacy = CODE_LEGACY
        ,Cpp_1 = 1
        ,Cpp_Current = Cpp_1
    };
}


#endif
