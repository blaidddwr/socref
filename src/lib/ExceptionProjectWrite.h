#ifndef EXCEPTION_PROJECT_WRITE_H
#define EXCEPTION_PROJECT_WRITE_H
#include "ExceptionBase.h"
namespace Exception {
namespace Project {




/*!
 * This is a block exception class. It represents a write project error.
 */
class Write:
    public Base
{
    using Base::Base;
};
}
}


#endif
