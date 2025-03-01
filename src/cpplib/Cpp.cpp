#include "Cpp.h"
#include <QStringList>
namespace Cpp {


const QString& codeKey(
    int index
)
{
    static const QStringList keys
    {
        "body"
        ,"footer"
        ,"footerHead"
        ,"preProcessHead"
        ,"preProcessSource"
        ,"header"
        ,"headerHead"
        ,"headerSource"
    };
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < keys.size());
    return keys.at(index);
}
}
