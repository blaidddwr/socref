#ifndef EXCEPTION_BLOCK_WRITE_H
#define EXCEPTION_BLOCK_WRITE_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is a block exception class. It represents a write block error.
 */
class Write:
    public Base
{
    using Base::Base;
};
}
}


#endif
