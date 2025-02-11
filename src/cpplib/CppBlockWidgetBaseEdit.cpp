#include "CppBlockWidgetBaseEdit.h"
#include <QtWidgets>
#include "ControllerSpellingHighlight.h"
#include "CppBlockBase.h"
namespace Cpp {
namespace BlockWidget {


BaseEdit::BaseEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    AbstractBlockWidget(block,parent)
{
}


bool BaseEdit::apply(
)
{
    auto block = qobject_cast<Block::Base*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    finishSet();
    setModified(false);
    return true;
}


QTextEdit* BaseEdit::descriptionTextEdit(
)
{
    if (!_descriptionEdit)
    {
        _descriptionEdit = new QTextEdit;
        new Controller::SpellingHighlight(_descriptionEdit->document());
        auto b = qobject_cast<const Block::Base*>(block());
        Q_ASSERT(b);
        _descriptionEdit->setText(b->description());
        connect(_descriptionEdit,&QTextEdit::textChanged,this,&BaseEdit::touch);
    }
    return _descriptionEdit;
}


QFormLayout* BaseEdit::formLayout(
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


QLineEdit* BaseEdit::nameLineEdit(
)
{
    if (!_nameEdit)
    {
        _nameEdit = new QLineEdit;
        auto b = qobject_cast<const Block::Base*>(block());
        Q_ASSERT(b);
        _nameEdit->setText(b->name());
        connect(_nameEdit,&QLineEdit::textChanged,this,&BaseEdit::touch);
    }
    return _nameEdit;
}
}
}
