#include "CppBlockException.h"
#include <QtGui>
#include "CppBlockWidgetExceptionEdit.h"
namespace Cpp {
namespace Block {


Exception::Exception(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("exception",meta,parent)
{
}


AbstractBlockWidget* Exception::createWidget(
) const
{
    return new BlockWidget::ExceptionEdit(this);
}


QIcon Exception::displayIcon(
) const
{
    return QIcon(":/cpp/exception.svg");
}


AbstractBlock* Exception::create(
    QObject* parent
) const
{
    return new Exception(meta(),parent);
}
}
}
