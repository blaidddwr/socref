#include "BlockCppExceptionItem.h"
#include <QtGui>
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


Block::Abstract* ExceptionItem::create(
    QObject* parent
) const
{
    return new ExceptionItem(meta(),parent);
}
}
}
