#include "WidgetBlockCppFunction.h"
#include <QtWidgets>
#include "BlockCppFunction.h"
#include "Exceptions.h"
#include "WidgetBlockAbstract.t.h"
#include "WidgetStringList.h"
namespace Widget {
namespace Block {
namespace Cpp {
using Block = ::Block::Cpp::Function;


Function::Function(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


void Function::apply(
)
{
    auto block = beginSet<Block>();
    try
    {
        int flags = 0;
        const auto& map = block->flagLabelMap();
        for(auto i = map.begin();i != map.end();i++)
        {
            if (flagCheckBox(i.key())->isChecked())
            {
                flags |= i.key();
            }
        }
        block->set(
            nameEdit()->text()
            ,returnLineEdit()->text()
            ,typeComboBox()->currentIndex()
            ,accessComboBox()->currentIndex()
            ,assignmentComboBox()->currentIndex()
            ,flags
        );
        block->setDescription(descriptionEdit()->toPlainText());
    }
    catch (Exception::Block::Logical& e)
    {
        QMessageBox::warning(this,tr("Logical Block Error"),e.message());
        abortSet();
        return;
    }
    finishSet();
    setModified(false);
}


QComboBox* Function::accessComboBox(
)
{
    if (!_accessComboBox)
    {
        auto b = block<Block>();
        _accessComboBox = createComboBox(b->accessLabels(),b->accessIcons(),b->access());
    }
    return _accessComboBox;
}


QComboBox* Function::assignmentComboBox(
)
{
    if (!_assignmentComboBox)
    {
        auto b = block<Block>();
        _assignmentComboBox = createComboBox(
            b->assignmentLabels()
            ,b->assignmentIcons()
            ,b->assignment()
        );
    }
    return _assignmentComboBox;
}


QComboBox* Function::createComboBox(
    const QStringList& labels
    ,const QList<QIcon>& icons
    ,int currentIndex
)
{
    auto ret = new QComboBox;
    G_ASSERT(labels.size() == icons.size());
    for (int i = 0;i < labels.size();i++)
    {
        ret->addItem(icons.at(i),labels.at(i));
    }
    ret->setCurrentIndex(currentIndex);
    connect(ret,&QComboBox::currentIndexChanged,this,&Function::touch);
    return ret;
}


QCheckBox* Function::flagCheckBox(
    int flag
)
{
    if (_flagCheckBoxes.isEmpty())
    {
        auto b = block<Block>();
        const auto& map = b->flagLabelMap();
        for(auto i = map.begin();i != map.end();i++)
        {
            auto checkBox = new QCheckBox(i.value());
            checkBox->setChecked(b->flags()&i.key());
            connect(checkBox,&QCheckBox::stateChanged,this,&Function::touch);
            _flagCheckBoxes.insert(i.key(),checkBox);
        }
    }
    G_ASSERT(_flagCheckBoxes.contains(flag));
    return _flagCheckBoxes.value(flag);
}


QGroupBox* Function::flagsGroupBox(
)
{
    if (!_flagsGroupBox)
    {
        _flagsGroupBox = new QGroupBox;
        auto layout = new QGridLayout;
        layout->setColumnStretch(FLAG_COL_SIZE-1,1);
        const auto& map = block<Block>()->flagLabelMap();
        int row = 0;
        int col = 0;
        for(auto i = map.begin();i != map.end();i++)
        {
            layout->addWidget(flagCheckBox(i.key()),row,col,Qt::AlignLeft);
            if (++col == FLAG_COL_SIZE)
            {
                col = 0;
                row++;
            }
        }
        _flagsGroupBox->setLayout(layout);
    }
    return _flagsGroupBox;
}


QFormLayout* Function::formLayout(
)
{
    if (!_formLayout)
    {
        Base::formLayout()->addRow(tr("Type:"),typeComboBox());
        Base::formLayout()->addRow(tr("Assignment:"),assignmentComboBox());
        Base::formLayout()->addRow(tr("Access:"),accessComboBox());
        Base::formLayout()->addRow(tr("Return:"),returnGroupBox());
        Base::formLayout()->addRow(tr("Flags:"),flagsGroupBox());
        Base::formLayout()->addRow(tr("Templates:"),templateList());
        _formLayout = true;
    }
    return Base::formLayout();
}


QTextEdit* Function::returnDescriptionTextEdit(
)
{
    if (!_returnDescriptionTextEdit)
    {
        _returnDescriptionTextEdit = new QTextEdit;
        _returnDescriptionTextEdit->setText(block<Block>()->returnDescription());
        connect(_returnDescriptionTextEdit,&QTextEdit::textChanged,this,&Function::touch);
    }
    return _returnDescriptionTextEdit;
}


QGroupBox* Function::returnGroupBox(
)
{
    if (!_returnGroupBox)
    {
        _returnGroupBox = new QGroupBox;
        auto layout = new QFormLayout;
        layout->addRow(tr("Type:"),returnLineEdit());
        layout->addRow(tr("Description:"),returnDescriptionTextEdit());
        _returnGroupBox->setLayout(layout);
    }
    return _returnGroupBox;
}


QLineEdit* Function::returnLineEdit(
)
{
    if (!_returnLineEdit)
    {
        _returnLineEdit = new QLineEdit;
        _returnLineEdit->setText(block<Block>()->returnType());
        connect(_returnLineEdit,&QLineEdit::textChanged,this,&Function::touch);
    }
    return _returnLineEdit;
}


StringList* Function::templateList(
)
{
    if (!_templateList)
    {
        _templateList = new StringList;
        _templateList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        _templateList->model()->setStringList(block<Block>()->templates());
        connect(_templateList,&StringList::stringListChanged,this,&Function::touch);
    }
    return _templateList;
}


QComboBox* Function::typeComboBox(
)
{
    if (!_typeComboBox)
    {
        auto b = block<Block>();
        _typeComboBox = createComboBox(b->typeLabels(),b->typeIcons(),b->type());
    }
    return _typeComboBox;
}
}
}
}
