#include "CppBlockException.h"
#include <QtGui>
#include "CppBlockFunction.h"
#include "CppBlockWidgetExceptionEdit.h"
namespace Cpp {
namespace Block {


Exception::Exception(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Namespace("exception",meta,parent)
{
}


AbstractBlockWidget* Exception::createWidget(
) const
{
    return new BlockWidget::ExceptionEdit(this);
}


QIcon Exception::displayIcon(
) const
{
    return QIcon(":/cpp/exception.svg");
}


void Exception::addEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}


AbstractBlock* Exception::create(
    QObject* parent
) const
{
    return new Exception(meta(),parent);
}


void Exception::removeEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}
}
}
