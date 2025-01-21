#include "BlockCppNamespace.h"
#include <QtGui>
#include "WidgetBlockCppNamespace.h"
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
) const
{
    return new Widget::Block::Cpp::Namespace(this);
}


QIcon Namespace::displayIcon(
) const
{
    return QIcon(":/cpp/namespace.svg");
}


Abstract* Namespace::create(
    QObject* parent
) const
{
    return new Namespace(meta(),parent);
}
}
}
