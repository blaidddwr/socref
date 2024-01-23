#include "BlockCppUnion.h"
#include <QtGui>
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
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
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
