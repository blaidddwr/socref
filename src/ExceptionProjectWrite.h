#ifndef EXCEPTION_PROJECT_WRITE_H
#define EXCEPTION_PROJECT_WRITE_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is an exception class. It represents a project writing error.
 */
class Write:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
