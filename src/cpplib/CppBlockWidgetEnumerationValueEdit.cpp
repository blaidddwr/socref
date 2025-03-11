#include "CppBlockWidgetEnumerationValueEdit.h"
#include <QtWidgets>
#include "CppBlockEnumerationValue.h"
namespace Cpp {
namespace BlockWidget {


EnumerationValueEdit::EnumerationValueEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    NamespaceEdit(block,parent)
{
    setLayout(formLayout());
}


bool EnumerationValueEdit::apply(
)
{
    auto block = qobject_cast<Block::EnumerationValue*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setAssignment(assignmentLineEdit()->text());
    finishSet();
    setModified(false);
    return true;
}


QLineEdit* EnumerationValueEdit::assignmentLineEdit(
)
{
    if (!_assignmentLineEdit)
    {
        _assignmentLineEdit = new QLineEdit;
        auto b = qobject_cast<const Block::EnumerationValue*>(block());
        Q_ASSERT(b);
        _assignmentLineEdit->setText(b->assignment());
        connect(_assignmentLineEdit,&QLineEdit::textChanged,this,&EnumerationValueEdit::touch);
    }
    return _assignmentLineEdit;
}


QFormLayout* EnumerationValueEdit::formLayout(
)
{
    if (!_formLayout)
    {
        NamespaceEdit::formLayout()->addRow(tr("Assignment:"),assignmentLineEdit());
        _formLayout = true;
    }
    return NamespaceEdit::formLayout();
}
}
}
