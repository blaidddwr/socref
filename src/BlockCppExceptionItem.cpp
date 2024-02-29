#include "BlockCppExceptionItem.h"
#include <QtGui>
#include "WidgetBlockCppExceptionItem.h"
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
) const
{
    return new Widget::Block::Cpp::ExceptionItem(this);
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
