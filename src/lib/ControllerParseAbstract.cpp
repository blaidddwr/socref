#include "ControllerParseAbstract.h"
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
namespace Controller {
namespace Parse {


Abstract::Abstract(
    Abstract* parent
):
    QObject(parent)
{
}


const QList<Abstract*>& Abstract::children(
)
{
    return _children;
}


void Abstract::setVersion(
    int value
)
{
    _version = value;
}


int Abstract::version(
) const
{
    Q_ASSERT(_version != -1);
    return _version;
}


void Abstract::addChild(
    Abstract* child
)
{
    Q_ASSERT(child);
    child->setParent(this);
    _children.append(child);
}
}
}
