#include "WidgetDialogUnitTests.h"
#include <QtWidgets>
#include "TestUnitModel.h"
#include "gassert.h"
#define SETTINGS_KEY "widget.dialog.unitTests"
namespace Widget {
namespace Dialog {


UnitTests::UnitTests(
    QWidget* parent
    ,Qt::WindowFlags flags
):
    QDialog(parent,flags)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(splitter());
    layout->addLayout(buttonsLayout());
    setLayout(layout);
    restoreGS();
}


void UnitTests::closeEvent(
    QCloseEvent* event
)
{
    saveGS();
    event->accept();
}


void UnitTests::hideEvent(
    QHideEvent* event
)
{
    saveGS();
    event->accept();
}


void UnitTests::onCurrentIndexChanged(
    const QModelIndex& current
    ,const QModelIndex& previous
)
{
    Q_UNUSED(previous);
    if (current.isValid())
    {
        outputTextEdit()->setText(model()->output(current));
        errorTextEdit()->setText(model()->error(current));
    }
}


void UnitTests::onDataChanged(
    const QModelIndex& topLeft
    ,const QModelIndex& bottomRight
    ,QList<int> roles
)
{
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
    if (
        topLeft.isValid()
        && topLeft.row() == tableView()->currentIndex().row()
    )
    {
        outputTextEdit()->setText(model()->output(topLeft));
        errorTextEdit()->setText(model()->error(topLeft));
    }
}


void UnitTests::run(
)
{
    auto index = tableView()->currentIndex();
    if (index.isValid())
    {
        model()->run(index);
    }
}


void UnitTests::runAll(
)
{
    model()->runAll();
}


QHBoxLayout* UnitTests::buttonsLayout(
)
{
    if (!_buttonsLayout)
    {
        _buttonsLayout = new QHBoxLayout;
        _buttonsLayout->addStretch();
        _buttonsLayout->addWidget(runButton());
        _buttonsLayout->addWidget(runAllButton());
        _buttonsLayout->addWidget(closeButton());
    }
    return _buttonsLayout;
}


QPushButton* UnitTests::closeButton(
)
{
    if (!_closeButton)
    {
        _closeButton = new QPushButton(tr("Close"));
        connect(_closeButton,&QPushButton::clicked,this,&UnitTests::accept);
    }
    return _closeButton;
}


QTextEdit* UnitTests::errorTextEdit(
)
{
    if (!_errorTextEdit)
    {
        _errorTextEdit = new QTextEdit;
        _errorTextEdit->setReadOnly(true);
    }
    return _errorTextEdit;
}


Test::UnitModel* UnitTests::model(
)
{
    if (!_model)
    {
        _model = new Test::UnitModel(this);
        connect(_model,&QAbstractItemModel::dataChanged,this,&UnitTests::onDataChanged);
    }
    return _model;
}


QTextEdit* UnitTests::outputTextEdit(
)
{
    if (!_outputTextEdit)
    {
        _outputTextEdit = new QTextEdit;
        _outputTextEdit->setReadOnly(true);
    }
    return _outputTextEdit;
}


void UnitTests::restoreGS(
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
    tableView()->restoreGeometry(read());
    tableView()->horizontalHeader()->restoreGeometry(read());
    tableView()->horizontalHeader()->restoreState(read());
}


QPushButton* UnitTests::runAllButton(
)
{
    if (!_runAllButton)
    {
        _runAllButton = new QPushButton(tr("Run All"));
        connect(_runAllButton,&QPushButton::clicked,this,&UnitTests::runAll);
    }
    return _runAllButton;
}


QPushButton* UnitTests::runButton(
)
{
    if (!_runButton)
    {
        _runButton = new QPushButton(tr("Run"));
        connect(_runButton,&QPushButton::clicked,this,&UnitTests::run);
    }
    return _runButton;
}


void UnitTests::saveGS(
) const
{
    G_ASSERT(_splitter);
    G_ASSERT(_tableView);
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
    write(_tableView->saveGeometry());
    write(_tableView->horizontalHeader()->saveGeometry());
    write(_tableView->horizontalHeader()->saveState());
    QSettings settings;
    settings.setValue(SETTINGS_KEY,data);
}


QSplitter* UnitTests::splitter(
)
{
    if (!_splitter)
    {
        _splitter = new QSplitter;
        _splitter->addWidget(tableView());
        _splitter->addWidget(tab());
    }
    return _splitter;
}


QTabWidget* UnitTests::tab(
)
{
    if (!_tab)
    {
        _tab = new QTabWidget;
        _tab->addTab(outputTextEdit(),tr("Standard Output"));
        _tab->addTab(errorTextEdit(),tr("Standard Error"));
    }
    return _tab;
}


QTableView* UnitTests::tableView(
)
{
    if (!_tableView)
    {
        _tableView = new QTableView;
        _tableView->verticalHeader()->hide();
        _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        _tableView->setModel(model());
        connect(
            _tableView->selectionModel()
            ,&QItemSelectionModel::currentChanged
            ,this
            ,&UnitTests::onCurrentIndexChanged
        );
    }
    return _tableView;
}
}
}
