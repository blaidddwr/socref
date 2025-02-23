#include "CppBlockUnion.h"
#include <QtGui>
#include "CppBlockWidgetUnionEdit.h"
namespace Cpp {
namespace Block {


Union::Union(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Namespace("union",meta,parent)
{
}


AbstractBlockWidget* Union::createWidget(
) const
{
    return new BlockWidget::UnionEdit(this);
}


QIcon Union::displayIcon(
) const
{
    return QIcon(":/cpp/union.svg");
}


AbstractBlock* Union::create(
    QObject* parent
) const
{
    return new Union(meta(),parent);
}
}
}
