#ifndef EXCEPTION_PROJECT_LOGICAL_H
#define EXCEPTION_PROJECT_LOGICAL_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is a block exception class. It represents a logical project error.
 */
class Logical:
    public Base
{
    using Base::Base;
};
}
}


#endif
