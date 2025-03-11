#include "CppBlockWidgetVariableEdit.h"
#include <QtWidgets>
#include "CppBlockVariable.h"
namespace Cpp {
namespace BlockWidget {


VariableEdit::VariableEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    NamespaceEdit(block,parent)
{
    setLayout(formLayout());
}


bool VariableEdit::apply(
)
{
    auto block = qobject_cast<Block::Variable*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setType(typeLineEdit()->text());
    block->setAssignment(assignmentLineEdit()->text());
    finishSet();
    setModified(false);
    return true;
}


QLineEdit* VariableEdit::assignmentLineEdit(
)
{
    if (!_assignmentLineEdit)
    {
        _assignmentLineEdit = new QLineEdit;
        auto b = qobject_cast<const Block::Variable*>(block());
        Q_ASSERT(b);
        _assignmentLineEdit->setText(b->assignment());
        connect(_assignmentLineEdit,&QLineEdit::textChanged,this,&VariableEdit::touch);
    }
    return _assignmentLineEdit;
}


QFormLayout* VariableEdit::formLayout(
)
{
    if (!_formLayout)
    {
        NamespaceEdit::formLayout()->addRow(tr("Type:"),typeLineEdit());
        NamespaceEdit::formLayout()->addRow(tr("Assignment:"),assignmentLineEdit());
        _formLayout = true;
    }
    return NamespaceEdit::formLayout();
}


QLineEdit* VariableEdit::typeLineEdit(
)
{
    if (!_typeLineEdit)
    {
        _typeLineEdit = new QLineEdit;
        auto b = qobject_cast<const Block::Variable*>(block());
        Q_ASSERT(b);
        _typeLineEdit->setText(b->type());
        connect(_typeLineEdit,&QLineEdit::textChanged,this,&VariableEdit::touch);
    }
    return _typeLineEdit;
}
}
}
