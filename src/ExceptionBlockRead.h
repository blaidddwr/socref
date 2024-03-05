#ifndef EXCEPTION_BLOCK_READ_H
#define EXCEPTION_BLOCK_READ_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is a block exception class. It represents a read block error.
 */
class Read:
    public Base
{
    using Base::Base;
};
}
}


#endif
