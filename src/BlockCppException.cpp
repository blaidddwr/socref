#include "BlockCppException.h"
#include <QtGui>
#include "WidgetBlockCppException.h"
namespace Block {
namespace Cpp {


Exception::Exception(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("exception",meta,parent)
{
}


Widget::Block::Abstract* Exception::createWidget(
) const
{
    return new Widget::Block::Cpp::Exception(this);
}


QIcon Exception::displayIcon(
) const
{
    return QIcon(":/cpp/exception.svg");
}


Abstract* Exception::create(
    QObject* parent
) const
{
    return new Exception(meta(),parent);
}
}
}
