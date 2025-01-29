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
    if (!_createdChildren)
    {
        auto language = Factory::Language::instance()->get(languageIndex());
        for (auto i: childrenIndexes())
        {
            _children.append(language->createParse(i,this));
        }
        _createdChildren = true;
    }
    return _children;
}
}
}
