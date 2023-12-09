#ifndef EXCEPTION_BLOCK_READ_H
#define EXCEPTION_BLOCK_READ_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is an exception class. It represents a block reading error.
 */
class Read:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
