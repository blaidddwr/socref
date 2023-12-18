#include "BlockCppNamespace.h"
#include <QtGui>
namespace Block {
namespace Cpp {


Namespace::Namespace(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("namespace",meta,parent)
{
}


Widget::Block::Abstract* Namespace::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Namespace::displayIcon(
) const
{
    return QIcon(":/cpp/namespace.svg");
}
}
}
