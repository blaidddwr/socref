#ifndef WIDGET_BLOCK_ABSTRACT_T_H
#define WIDGET_BLOCK_ABSTRACT_T_H
#include "WidgetBlockAbstract.h"
#include "ModelProject.h"
#include "gassert.h"
namespace Widget {
namespace Block {


template<class T> T* Abstract::beginSet(
)
{
    G_ASSERT(_model);
    G_ASSERT(_index.isValid());
    G_ASSERT(_index.model() == _model);
    G_ASSERT(_block);
    G_ASSERT(_index.internalPointer() == _block);
    auto ret = qobject_cast<T*>(_model->beginSet(_index));
    G_ASSERT(ret == _block);
    return ret;
}


template<class T> const T* Abstract::block(
)
{
    auto ret = qobject_cast<const T*>(_block);
    G_ASSERT(ret);
    return ret;
}
}
}


#endif
