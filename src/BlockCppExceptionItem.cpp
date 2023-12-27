#include "BlockCppExceptionItem.h"
#include <QtGui>
#include "Exception.h"
namespace Block {
namespace Cpp {


ExceptionItem::ExceptionItem(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("exception",meta,parent)
{
}


Widget::Block::Abstract* ExceptionItem::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon ExceptionItem::displayIcon(
) const
{
    return QIcon(":/cpp/exception.svg");
}
}
}
