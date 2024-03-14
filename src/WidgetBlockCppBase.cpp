#include "WidgetBlockCppBase.h"
#include <QtWidgets>
#include "BlockCppBase.h"
#include "WidgetBlockAbstract.t.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Base;


Base::Base(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Abstract(block,parent)
{
}


bool Base::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    finishSet();
    setModified(false);
    return true;
}


QTextEdit* Base::descriptionTextEdit(
)
{
    if (!_descriptionEdit)
    {
        _descriptionEdit = new QTextEdit;
        _descriptionEdit->setText(block<Block>()->description());
        connect(_descriptionEdit,&QTextEdit::textChanged,this,&Base::touch);
    }
    return _descriptionEdit;
}


QFormLayout* Base::formLayout(
)
{
    if (!_formLayout)
    {
        _formLayout = new QFormLayout;
        _formLayout->addRow(tr("Name:"),nameLineEdit());
        _formLayout->addRow(tr("Description:"),descriptionTextEdit());
    }
    return _formLayout;
}


QLineEdit* Base::nameLineEdit(
)
{
    if (!_nameEdit)
    {
        _nameEdit = new QLineEdit;
        _nameEdit->setText(block<Block>()->name());
        connect(_nameEdit,&QLineEdit::textChanged,this,&Base::touch);
    }
    return _nameEdit;
}
}
}
}
