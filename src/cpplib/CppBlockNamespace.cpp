#include "CppBlockNamespace.h"
#include <QtGui>
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace Block {


Namespace::Namespace(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("namespace",meta,parent)
{
}


AbstractBlockWidget* Namespace::createWidget(
) const
{
    return new BlockWidget::NamespaceEdit(this);
}


QIcon Namespace::displayIcon(
) const
{
    return QIcon(":/cpp/namespace.svg");
}


AbstractBlock* Namespace::create(
    QObject* parent
) const
{
    return new Namespace(meta(),parent);
}
}
}
