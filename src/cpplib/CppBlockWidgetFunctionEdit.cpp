#include "CppBlockWidgetFunctionEdit.h"
#include <QtWidgets>
#include "CppBlockFunction.h"
#include "ControllerSpellingHighlight.h"
#include "Exception.h"
#include "WidgetStringList.h"
namespace Cpp {
namespace BlockWidget {


FunctionEdit::FunctionEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}


bool FunctionEdit::apply(
)
{
    auto block = qobject_cast<Block::Function*>(beginSet());
    Q_ASSERT(block);
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
            nameLineEdit()->text()
            ,returnLineEdit()->text()
            ,typeComboBox()->currentIndex()
            ,accessComboBox()->currentIndex()
            ,assignmentComboBox()->currentIndex()
            ,flags
            );
        block->setDescription(descriptionTextEdit()->toPlainText());
        block->setReturnDescription(returnDescriptionTextEdit()->toPlainText());
        block->setTemplates(templateStringList()->model()->stringList());
    }
    catch (Exception::LogicalBlock& e)
    {
        QMessageBox::warning(this,tr("Logical Block Error"),e.message());
        abortSet();
        return false;
    }
    finishSet();
    setModified(false);
    return true;
}


QComboBox* FunctionEdit::accessComboBox(
)
{
    if (!_accessComboBox)
    {
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _accessComboBox = createComboBox(b->accessLabels(),b->accessIcons(),b->access());
    }
    return _accessComboBox;
}


QComboBox* FunctionEdit::assignmentComboBox(
)
{
    if (!_assignmentComboBox)
    {
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _assignmentComboBox = createComboBox(
            b->assignmentLabels()
            ,b->assignmentIcons()
            ,b->assignment()
            );
    }
    return _assignmentComboBox;
}


QComboBox* FunctionEdit::createComboBox(
    const QStringList& labels
    ,const QList<QIcon>& icons
    ,int currentIndex
)
{
    auto ret = new QComboBox;
    Q_ASSERT(labels.size() == icons.size());
    for (int i = 0;i < labels.size();i++)
    {
        ret->addItem(icons.at(i),labels.at(i));
    }
    ret->setCurrentIndex(currentIndex);
    connect(ret,&QComboBox::currentIndexChanged,this,&FunctionEdit::touch);
    return ret;
}


QCheckBox* FunctionEdit::flagCheckBox(
    int flag
)
{
    if (_flagCheckBoxes.isEmpty())
    {
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        const auto& map = b->flagLabelMap();
        for(auto i = map.begin();i != map.end();i++)
        {
            auto checkBox = new QCheckBox(i.value());
            checkBox->setChecked(b->flags()&i.key());
            connect(checkBox,&QCheckBox::checkStateChanged,this,&FunctionEdit::touch);
            _flagCheckBoxes.insert(i.key(),checkBox);
        }
    }
    Q_ASSERT(_flagCheckBoxes.contains(flag));
    return _flagCheckBoxes.value(flag);
}


QGroupBox* FunctionEdit::flagsGroupBox(
)
{
    if (!_flagsGroupBox)
    {
        _flagsGroupBox = new QGroupBox;
        auto layout = new QGridLayout;
        layout->setColumnStretch(FLAG_COL_SIZE-1,1);
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        const auto& map = b->flagLabelMap();
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


QFormLayout* FunctionEdit::formLayout(
)
{
    if (!_formLayout)
    {
        BaseEdit::formLayout()->addRow(tr("Type:"),typeComboBox());
        BaseEdit::formLayout()->addRow(tr("Assignment:"),assignmentComboBox());
        BaseEdit::formLayout()->addRow(tr("Access:"),accessComboBox());
        BaseEdit::formLayout()->addRow(tr("Return:"),returnGroupBox());
        BaseEdit::formLayout()->addRow(tr("Flags:"),flagsGroupBox());
        BaseEdit::formLayout()->addRow(tr("Templates:"),templateStringList());
        _formLayout = true;
    }
    return BaseEdit::formLayout();
}


QTextEdit* FunctionEdit::returnDescriptionTextEdit(
)
{
    if (!_returnDescriptionTextEdit)
    {
        _returnDescriptionTextEdit = new QTextEdit;
        new Controller::SpellingHighlight(_returnDescriptionTextEdit->document());
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _returnDescriptionTextEdit->setText(b->returnDescription());
        connect(_returnDescriptionTextEdit,&QTextEdit::textChanged,this,&FunctionEdit::touch);
    }
    return _returnDescriptionTextEdit;
}


QGroupBox* FunctionEdit::returnGroupBox(
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


QLineEdit* FunctionEdit::returnLineEdit(
)
{
    if (!_returnLineEdit)
    {
        _returnLineEdit = new QLineEdit;
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _returnLineEdit->setText(b->returnType());
        connect(_returnLineEdit,&QLineEdit::textChanged,this,&FunctionEdit::touch);
    }
    return _returnLineEdit;
}


Widget::StringList* FunctionEdit::templateStringList(
)
{
    if (!_templateList)
    {
        _templateList = new ::Widget::StringList;
        _templateList->setMaximumHeight(QFontMetrics(QFont()).height()*4);
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _templateList->model()->setStringList(b->templates());
        connect(_templateList,&::Widget::StringList::stringListChanged,this,&FunctionEdit::touch);
    }
    return _templateList;
}


QComboBox* FunctionEdit::typeComboBox(
)
{
    if (!_typeComboBox)
    {
        auto b = qobject_cast<const Block::Function*>(block());
        Q_ASSERT(b);
        _typeComboBox = createComboBox(b->typeLabels(),b->typeIcons(),b->type());
    }
    return _typeComboBox;
}
}
}
