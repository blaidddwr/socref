#ifndef EXCEPTION_BLOCK_LOGICAL_H
#define EXCEPTION_BLOCK_LOGICAL_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is an exception class. It represents a logical block error.
 */
class Logical:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif
