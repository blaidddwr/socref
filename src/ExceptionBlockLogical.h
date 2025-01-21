#ifndef EXCEPTION_BLOCK_LOGICAL_H
#define EXCEPTION_BLOCK_LOGICAL_H
#include "ExceptionBase.h"
namespace Exception {
namespace Block {




/*!
 * This is a block exception class. It represents a logical block error.
 */
class Logical:
    public Base
{
    using Base::Base;
};
}
}


#endif
