#ifndef CONTROLLER_CODE_ABSTRACTPARSER_T_H
#define CONTROLLER_CODE_ABSTRACTPARSER_T_H
#include "ControllerCodeAbstractParser.h"
namespace Controller {
namespace Code {


template<class T> T* AbstractParser::block(
) const
{
    auto ret = qobject_cast<T*>(_block);
    Q_ASSERT(ret);
    return ret;
}
}
}


#endif
