#include "CppBlockWidgetNamespaceEdit.h"
#include <QtWidgets>
#include "ControllerSpellingHighlight.h"
#include "CppBlockNamespace.h"
namespace Cpp {
namespace BlockWidget {


NamespaceEdit::NamespaceEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    AbstractBlockWidget(block,parent)
{
    setLayout(formLayout());
}


bool NamespaceEdit::apply(
)
{
    auto block = qobject_cast<Block::Namespace*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    finishSet();
    setModified(false);
    return true;
}


QTextEdit* NamespaceEdit::descriptionTextEdit(
)
{
    if (!_descriptionEdit)
    {
        _descriptionEdit = new QTextEdit;
        new Controller::SpellingHighlight(_descriptionEdit->document());
        auto b = qobject_cast<const Block::Namespace*>(block());
        Q_ASSERT(b);
        _descriptionEdit->setText(b->description());
        connect(_descriptionEdit,&QTextEdit::textChanged,this,&NamespaceEdit::touch);
    }
    return _descriptionEdit;
}


QFormLayout* NamespaceEdit::formLayout(
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


QLineEdit* NamespaceEdit::nameLineEdit(
)
{
    if (!_nameEdit)
    {
        _nameEdit = new QLineEdit;
        auto b = qobject_cast<const Block::Namespace*>(block());
        Q_ASSERT(b);
        _nameEdit->setText(b->name());
        connect(_nameEdit,&QLineEdit::textChanged,this,&NamespaceEdit::touch);
    }
    return _nameEdit;
}
}
}
