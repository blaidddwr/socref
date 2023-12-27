#include "BlockCppProperty.h"
#include <QtGui>
namespace Block {
namespace Cpp {


Property::Property(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("property",meta,parent)
{
}


Widget::Block::Abstract* Property::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Property::displayIcon(
) const
{
    return QIcon(":/cpp/property.svg");
}
}
}
