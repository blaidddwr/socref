#include "BlockCppUnion.h"
#include <QtGui>
#include "WidgetBlockCppUnion.h"
namespace Block {
namespace Cpp {


Union::Union(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("union",meta,parent)
{
}


Widget::Block::Abstract* Union::createWidget(
) const
{
    return new Widget::Block::Cpp::Union(this);
}


QIcon Union::displayIcon(
) const
{
    return QIcon(":/cpp/union.svg");
}


Block::Abstract* Union::create(
    QObject* parent
) const
{
    return new Union(meta(),parent);
}
}
}
