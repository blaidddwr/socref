#include "WidgetBlockCppClass.h"
#include <QtWidgets>
#include "BlockCppClass.h"
#include "WidgetBlockAbstract.t.h"
#include "WidgetStringList.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Class;


Class::Class(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


void Class::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setParents(parentStringList()->model()->stringList());
    block->setTemplates(templateStringList()->model()->stringList());
    finishSet();
    setModified(false);
}


QFormLayout* Class::formLayout(
)
{
    if (!_formLayout)
    {
        Base::formLayout()->addRow(tr("Parents:"),parentStringList());
        Base::formLayout()->addRow(tr("Templates:"),templateStringList());
        _formLayout = true;
    }
    return Base::formLayout();
}


StringList* Class::parentStringList(
)
{
    if (!_parentList)
    {
        _parentList = new StringList;
        _parentList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        _parentList->model()->setStringList(block<Block>()->parents());
        connect(_parentList,&StringList::stringListChanged,this,&Class::touch);
    }
    return _parentList;
}


StringList* Class::templateStringList(
)
{
    if (!_templateList)
    {
        _templateList = new StringList;
        _templateList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        _templateList->model()->setStringList(block<Block>()->templates());
        connect(_templateList,&StringList::stringListChanged,this,&Class::touch);
    }
    return _templateList;
}
}
}
}
