#ifndef EXCEPTION_BLOCK_WRITE_H
#define EXCEPTION_BLOCK_WRITE_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is an exception class. It represents a block writing error.
 */
class Write:
    public Exception::Base
{
    using Exception::Base::Base;
};
}
}


#endif