#include "CppBlockWidgetClassEdit.h"
#include <QtWidgets>
#include "CppBlockClass.h"
#include "WidgetStringList.h"
namespace Cpp {
namespace BlockWidget {


ClassEdit::ClassEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    PropertyEdit(block,parent)
{
    setLayout(formLayout());
}


bool ClassEdit::apply(
)
{
    auto block = qobject_cast<Block::Class*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setParents(parentStringList()->model()->stringList());
    block->setTemplates(templateStringList()->model()->stringList());
    finishSet();
    setModified(false);
    return true;
}


QFormLayout* ClassEdit::formLayout(
)
{
    if (!_formLayout)
    {
        NamespaceEdit::formLayout()->addRow(tr("Parents:"),parentStringList());
        NamespaceEdit::formLayout()->addRow(tr("Templates:"),templateStringList());
        _formLayout = true;
    }
    return NamespaceEdit::formLayout();
}


Widget::StringList* ClassEdit::parentStringList(
)
{
    if (!_parentList)
    {
        _parentList = new ::Widget::StringList;
        _parentList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        auto b = qobject_cast<const Block::Class*>(block());
        Q_ASSERT(b);
        _parentList->model()->setStringList(b->parents());
        connect(_parentList,&Widget::StringList::stringListChanged,this,&ClassEdit::touch);
    }
    return _parentList;
}


Widget::StringList* ClassEdit::templateStringList(
)
{
    if (!_templateList)
    {
        _templateList = new ::Widget::StringList;
        _templateList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        auto b = qobject_cast<const Block::Class*>(block());
        Q_ASSERT(b);
        _templateList->model()->setStringList(b->templates());
        connect(_templateList,&Widget::StringList::stringListChanged,this,&ClassEdit::touch);
    }
    return _templateList;
}
}
}
