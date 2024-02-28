#include "WidgetBlockCppEnumeration.h"
#include <QtWidgets>
#include "BlockCppEnumeration.h"
#include "WidgetBlockAbstract.t.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Enumeration;


Enumeration::Enumeration(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


void Enumeration::apply(
)
{
    auto block = beginSet<Block>();
    block->setName(nameEdit()->text());
    block->setDescription(descriptionEdit()->toPlainText());
    block->setClass(classCheckBox()->isChecked());
    finishSet();
    setModified(false);
}


QCheckBox* Enumeration::classCheckBox(
)
{
    if (!_classCheckBox)
    {
        _classCheckBox = new QCheckBox(tr("Class"));
        _classCheckBox->setChecked(block<Block>()->isClass());
        connect(_classCheckBox,&QCheckBox::stateChanged,this,&Enumeration::touch);
    }
    return _classCheckBox;
}


QGroupBox* Enumeration::flagsGroupBox(
)
{
    if (!_flagsGroupBox)
    {
        _flagsGroupBox = new QGroupBox;
        auto layout = new QGridLayout;
        layout->addWidget(classCheckBox());
        _flagsGroupBox->setLayout(layout);
    }
    return _flagsGroupBox;
}


QFormLayout* Enumeration::formLayout(
)
{
    if (!_formLayout)
    {
        Base::formLayout()->addRow(tr("Flags:"),flagsGroupBox());
        _formLayout = true;
    }
    return Base::formLayout();
}
}
}
}
