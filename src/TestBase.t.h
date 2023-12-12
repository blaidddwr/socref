#ifndef TEST_BASE_T_H
#define TEST_BASE_T_H
#include "TestBase.h"
#include "Exception.h"
#include "LanguageAbstract.h"
namespace Test {


template<class T> T* Base::create(
    int index
)
{
    G_ASSERT(_language);
    auto ret = qobject_cast<T*>(_language->create(index,this));
    G_ASSERT(ret);
    return ret;
}
}


#endif
