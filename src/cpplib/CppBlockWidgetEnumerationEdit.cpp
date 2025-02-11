#include "CppBlockWidgetEnumerationEdit.h"
#include <QtWidgets>
#include "CppBlockEnumeration.h"
namespace Cpp {
namespace BlockWidget {


EnumerationEdit::EnumerationEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}


bool EnumerationEdit::apply(
)
{
    auto block = qobject_cast<Block::Enumeration*>(beginSet());
    Q_ASSERT(block);
    block->setName(nameLineEdit()->text());
    block->setDescription(descriptionTextEdit()->toPlainText());
    block->setClass(classCheckBox()->isChecked());
    finishSet();
    setModified(false);
    return true;
}


QCheckBox* EnumerationEdit::classCheckBox(
)
{
    if (!_classCheckBox)
    {
        _classCheckBox = new QCheckBox(tr("Class"));
        auto b = qobject_cast<const Block::Enumeration*>(block());
        Q_ASSERT(b);
        _classCheckBox->setChecked(b->isClass());
        connect(_classCheckBox,&QCheckBox::checkStateChanged,this,&EnumerationEdit::touch);
    }
    return _classCheckBox;
}


QGroupBox* EnumerationEdit::flagsGroupBox(
)
{
    if (!_flagsGroupBox)
    {
        _flagsGroupBox = new QGroupBox;
        auto layout = new QHBoxLayout;
        layout->addWidget(classCheckBox());
        layout->addStretch();
        _flagsGroupBox->setLayout(layout);
    }
    return _flagsGroupBox;
}


QFormLayout* EnumerationEdit::formLayout(
)
{
    if (!_formLayout)
    {
        BaseEdit::formLayout()->addRow(tr("Flags:"),flagsGroupBox());
        _formLayout = true;
    }
    return BaseEdit::formLayout();
}
}
}
