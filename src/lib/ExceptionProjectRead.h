#ifndef EXCEPTION_PROJECT_READ_H
#define EXCEPTION_PROJECT_READ_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is a block exception class. It represents a read project error.
 */
class Read:
    public Base
{
    using Base::Base;
};
}
}


#endif
