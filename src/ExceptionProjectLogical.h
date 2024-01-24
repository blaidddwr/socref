#ifndef EXCEPTION_PROJECT_LOGICAL_H
#define EXCEPTION_PROJECT_LOGICAL_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is an exception class. It represents a logical project error.
 */
class Logical:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
