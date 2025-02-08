#ifndef CONTROLLER_PARSE_ABSTRACT_T_H
#define CONTROLLER_PARSE_ABSTRACT_T_H
#include "ControllerParseAbstract.h"
namespace Controller {
namespace Parse {


template<class T> T* Abstract::block(
) const
{
    auto ret = qobject_cast<T*>(_block);
    Q_ASSERT(ret);
    return ret;
}
}
}


#endif
