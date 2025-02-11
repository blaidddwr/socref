#include "CppBlockNamespace.h"
#include <QtGui>
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace Block {
const char* Namespace::PREPROCESS_IN_HEADER_CODE_KEY = "preProcessInHeaderFile";
const char* Namespace::HEADER_IN_HEADER_CODE_KEY = "headerInHeaderFile";


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
