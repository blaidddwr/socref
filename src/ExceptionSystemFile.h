#ifndef EXCEPTION_SYSTEM_FILE_H
#define EXCEPTION_SYSTEM_FILE_H
#include "ExceptionBase.h"
namespace Exception {
namespace System {




/*!
 * This is an exception class. It represents a file system error.
 */
class File:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
