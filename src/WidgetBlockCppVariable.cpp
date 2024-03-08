#include "WidgetBlockCppVariable.h"
#include <QtWidgets>
#include "BlockCppVariable.h"
#include "WidgetBlockAbstract.t.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Variable;


Variable::Variable(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


bool Variable::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setType(typeLineEdit()->text());
    block->setAssignment(assignmentLineEdit()->text());
    finishSet();
    setModified(false);
    return true;
}


QLineEdit* Variable::assignmentLineEdit(
)
{
    if (!_assignmentLineEdit)
    {
        _assignmentLineEdit = new QLineEdit;
        _assignmentLineEdit->setText(block<Block>()->type());
        connect(_assignmentLineEdit,&QLineEdit::textChanged,this,&Variable::touch);
    }
    return _assignmentLineEdit;
}


QFormLayout* Variable::formLayout(
)
{
    if (!_formLayout)
    {
        Base::formLayout()->addRow(tr("Type:"),typeLineEdit());
        Base::formLayout()->addRow(tr("Assignment:"),assignmentLineEdit());
        _formLayout = true;
    }
    return Base::formLayout();
}


QLineEdit* Variable::typeLineEdit(
)
{
    if (!_typeLineEdit)
    {
        _typeLineEdit = new QLineEdit;
        _typeLineEdit->setText(block<Block>()->assignment());
        connect(_typeLineEdit,&QLineEdit::textChanged,this,&Variable::touch);
    }
    return _typeLineEdit;
}
}
}
}
