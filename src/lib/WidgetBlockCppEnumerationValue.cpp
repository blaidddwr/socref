#include "WidgetBlockCppEnumerationValue.h"
#include <QtWidgets>
#include "BlockCppEnumerationValue.h"
#include "WidgetBlockAbstract.t.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::EnumerationValue;


EnumerationValue::EnumerationValue(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


bool EnumerationValue::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setAssignment(assignmentLineEdit()->text());
    finishSet();
    setModified(false);
    return true;
}


QLineEdit* EnumerationValue::assignmentLineEdit(
)
{
    if (!_assignmentLineEdit)
    {
        _assignmentLineEdit = new QLineEdit;
        _assignmentLineEdit->setText(block<Block>()->assignment());
        connect(_assignmentLineEdit,&QLineEdit::textChanged,this,&EnumerationValue::touch);
    }
    return _assignmentLineEdit;
}


QFormLayout* EnumerationValue::formLayout(
)
{
    if (!_formLayout)
    {
        Base::formLayout()->addRow(tr("Assignment:"),assignmentLineEdit());
        _formLayout = true;
    }
    return Base::formLayout();
}
}
}
}
