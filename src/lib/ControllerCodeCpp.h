#ifndef CONTROLLER_CODE_CPP_H
#define CONTROLLER_CODE_CPP_H
#include "ControllerCode.h"
namespace Controller {
namespace Code {


/*!
 * This contains all C++ code controllers.
 */
namespace Cpp
{
    class HeadParser;


    /*!
     * This enumerates all versions for C++ source code parsing and building.
     */
    enum Version
    {
        Cpp_Legacy = Code_Legacy
        ,Cpp_1 = 1
        ,Cpp_Current = Cpp_1
    };
}
}
}


#endif
