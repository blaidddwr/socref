#include "WidgetDialogCode.h"
#include <QtWidgets>
#include "AbstractBlock.h"
namespace Widget {
namespace Dialog {


Code::Code(
    const AbstractBlock* block
    ,QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
    ,_block(block)
{
    Q_ASSERT(block);
    connect(
        const_cast<AbstractBlock*>(_block)
        ,&QObject::destroyed
        ,this
        ,&Code::onBlockDestroyed
        );
    connect(
        const_cast<AbstractBlock*>(_block)
        ,&AbstractBlock::codeChanged
        ,this
        ,&Code::onCodeChanged
        );
    auto layout = new QVBoxLayout;
    layout->addLayout(viewLayout());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    onCodeChanged(_block->code());
}


void Code::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        _block = nullptr;
    }
}


void Code::onCodeChanged(
    const QMap<QString,QStringList>& value
)
{
    stringListModel()->setStringList(value.keys());
    textEdit()->setPlainText("");
    if (!value.isEmpty())
    {
        listView()->selectionModel()->setCurrentIndex(
            stringListModel()->index(0)
            ,QItemSelectionModel::Clear|QItemSelectionModel::Current
            );
        textEdit()->setPlainText(value.first().join("\n"));
    }
}


QHBoxLayout* Code::buttonsLayout(
)
{
    if (!_buttonsLayout)
    {
        _buttonsLayout = new QHBoxLayout;
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(closeButton());
    }
    return _buttonsLayout;
}


QPushButton* Code::closeButton(
)
{
    if (!_closeButton)
    {
        _closeButton = new QPushButton(tr("Close"));
        connect(_closeButton,&QPushButton::clicked,this,&QWidget::close);
    }
    return _closeButton;
}


QListView* Code::listView(
)
{
    if (!_listView)
    {
        _listView = new QListView;
        _listView->setModel(stringListModel());
    }
    return _listView;
}


QStringListModel* Code::stringListModel(
)
{
    if (!_stringListModel)
    {
        _stringListModel = new QStringListModel(this);
    }
    return _stringListModel;
}


QPlainTextEdit* Code::textEdit(
)
{
    if (!_textEdit)
    {
        _textEdit = new QPlainTextEdit;
        _textEdit->setReadOnly(true);
        auto font = _textEdit->font();
        font.setFamily("monospace");
        _textEdit->setFont(font);
    }
    return _textEdit;
}


QHBoxLayout* Code::viewLayout(
)
{
    if (!_viewLayout)
    {
        _viewLayout = new QHBoxLayout;
        _viewLayout->addWidget(listView());
        _viewLayout->addWidget(textEdit());
    }
    return _viewLayout;
}
}
}
