#ifndef EXCEPTION_PROJECT_READ_H
#define EXCEPTION_PROJECT_READ_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is an exception class. It represents a project reading error.
 */
class Read:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
