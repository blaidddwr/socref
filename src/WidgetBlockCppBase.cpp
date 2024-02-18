#include "WidgetBlockCppBase.h"
#include <QtWidgets>
#include "BlockCppBase.h"
#include "WidgetBlockAbstract.t.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Base;


void Base::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameEdit()->text());
    block->setDescription(descriptionEdit()->toPlainText());
    finishSet();
    setModified(false);
}


QTextEdit* Base::descriptionEdit(
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
        _formLayout->addRow(tr("Name:"),nameEdit());
        _formLayout->addRow(tr("Description:"),descriptionEdit());
    }
    return _formLayout;
}


QLineEdit* Base::nameEdit(
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
