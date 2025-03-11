#include "WidgetDialogCode.h"
#include <QtWidgets>
#include "AbstractBlock.h"
#include "ModelCode.h"
#define SETTINGS_KEY "widget.dialog.code"
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
    auto layout = new QVBoxLayout;
    layout->addWidget(splitter());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    connect(const_cast<AbstractBlock*>(block),&QObject::destroyed,this,&Code::onBlockDestroyed);
    restore();
}


void Code::closeEvent(
    QCloseEvent* event
)
{
    save();
    event->accept();
}


void Code::hideEvent(
    QHideEvent* event
)
{
    save();
    event->accept();
}


void Code::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        _block = nullptr;
        model()->setCode({});
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
        _listView->setModel(model());
        connect(
            _listView->selectionModel()
            ,&QItemSelectionModel::currentChanged
            ,model()
            ,&Model::Code::setCurrentIndex
            );
    }
    return _listView;
}


Model::Code* Code::model(
)
{
    if (!_model)
    {
        _model = new Model::Code(this);
        connect(
            const_cast<AbstractBlock*>(_block)
            ,&AbstractBlock::codeChanged
            ,_model
            ,&Model::Code::setCode
            );
        _model->setCode(_block->code());
    }
    return _model;
}


void Code::restore(
)
{
    QSettings settings;
    auto data = settings.value(SETTINGS_KEY).toByteArray();
    QDataStream in(data);
    auto read = [&in]() -> QByteArray {
        QByteArray ret;
        qint32 size;
        in >> size;
        ret.resize(size);
        in.readRawData(ret.data(),size);
        return ret;
    };
    restoreGeometry(read());
    splitter()->restoreGeometry(read());
    splitter()->restoreState(read());
}


void Code::save(
) const
{
    Q_ASSERT(_splitter);
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    auto write = [&out](const QByteArray& subset) {
        qint32 size = subset.size();
        out << size;
        out.writeRawData(subset.constData(),size);
    };
    write(saveGeometry());
    write(_splitter->saveGeometry());
    write(_splitter->saveState());
    QSettings settings;
    settings.setValue(SETTINGS_KEY,data);
}


QSplitter* Code::splitter(
)
{
    if (!_splitter)
    {
        _splitter = new QSplitter;
        _splitter->setChildrenCollapsible(false);
        _splitter->addWidget(listView());
        _splitter->addWidget(textEdit());
    }
    return _splitter;
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
        connect(model(),&Model::Code::textChanged,_textEdit,&QPlainTextEdit::setPlainText);
    }
    return _textEdit;
}
}
}
