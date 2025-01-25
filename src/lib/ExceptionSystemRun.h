#ifndef EXCEPTION_SYSTEM_RUN_H
#define EXCEPTION_SYSTEM_RUN_H
#include "ExceptionBase.h"
namespace Exception {
namespace System {




/*!
 * This is a block exception class. It represents a run system error.
 */
class Run:
    public Base
{
    using Base::Base;
};
}
}


#endif
