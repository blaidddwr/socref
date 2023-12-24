#include "BlockCppException.h"
#include <QtGui>
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
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Exception::displayIcon(
) const
{
    return QIcon(":/cpp/exception.svg");
}
}
}
