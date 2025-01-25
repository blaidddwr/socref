#include "TestUnitModel.h"
#include <QtGui>
#include "Test.h"
#include "gassert.h"
namespace Test {


UnitModel::UnitModel(
    QObject* parent
):
    QAbstractTableModel(parent)
{
    for (const auto& test: Test::testObjects())
    {
        _tests.append({test.name,UnitTest::Result::None,"",""});
    }
}


int UnitModel::columnCount(
    const QModelIndex& parent
) const
{
    if (parent.isValid())
    {
        return 0;
    }
    else
    {
        return ColumnTotal;
    }
}


QVariant UnitModel::data(
    const QModelIndex& index
    ,int role
) const
{
    using Result = UnitTest::Result;
    G_ASSERT(index.isValid());
    G_ASSERT(index.row() >= 0);
    G_ASSERT(index.row() < _tests.size());
    switch (index.column())
    {
    case NameColumn:
        switch (role)
        {
        case Qt::DisplayRole:
            return _tests.at(index.row()).name;
        default:
            return QVariant();
        }
    case ResultColumn:
        switch (role)
        {
        case Qt::DisplayRole:
            switch (_tests.at(index.row()).result)
            {
            case Result::Failed:
                return tr("FAILED");
            case Result::None:
                return tr("NONE");
            case Result::Passed:
                return tr("PASS");
            case Result::Running:
                return tr("RUNNING");
            default:
                return QVariant();
            }
        case Qt::FontRole:
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        case Qt::ForegroundRole:
            return QColor(Qt::black);
        case Qt::BackgroundRole:
            switch (_tests.at(index.row()).result)
            {
            case Result::Failed:
                return QColor(255,100,100);
            case Result::None:
                return QColor(255,255,100);
            case Result::Passed:
                return QColor(100,255,100);
            case Result::Running:
                return QColor(100,100,255);
            default:
                return QVariant();
            }
        default:
            return QVariant();
        }

    default:
        return QVariant();
    }
}


const QString& UnitModel::error(
    const QModelIndex& index
) const
{
    G_ASSERT(index.isValid());
    G_ASSERT(index.model() == this);
    G_ASSERT(index.row() >= 0);
    G_ASSERT(index.row() < _tests.size());
    return _tests.at(index.row()).error;
}


QVariant UnitModel::headerData(
    int section
    ,Qt::Orientation orientation
    ,int role
) const
{
    if (
        orientation == Qt::Horizontal
        && role == Qt::DisplayRole
    )
    {
        switch (section)
        {
        case NameColumn:
            return tr("Name");
        case ResultColumn:
            return tr("Result");
        default:
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}


const QString& UnitModel::output(
    const QModelIndex& index
) const
{
    G_ASSERT(index.isValid());
    G_ASSERT(index.model() == this);
    G_ASSERT(index.row() >= 0);
    G_ASSERT(index.row() < _tests.size());
    return _tests.at(index.row()).output;
}


int UnitModel::rowCount(
    const QModelIndex& parent
) const
{
    if (parent.isValid())
    {
        return 0;
    }
    else
    {
        return _tests.size();
    }
}


void UnitModel::run(
    const QModelIndex& index
)
{
    using Result = UnitTest::Result;
    G_ASSERT(index.isValid());
    G_ASSERT(index.model() == this);
    G_ASSERT(index.row() >= 0);
    G_ASSERT(index.row() < _tests.size());
    auto test = &_tests[index.row()];
    if (test->result == Result::Running)
    {
        return;
    }
    auto ni = createIndex(index.row(),ResultColumn);
    auto process = new QProcess(this);
    connect(
        process
        ,&QProcess::finished
        ,this
        ,[this,test,ni,process](int exitCode)
        {
            test->output = process->readAllStandardOutput();
            test->error = process->readAllStandardError();
            test->result = exitCode ? Result::Failed : Result::Passed;
            emit dataChanged(ni,ni,{Qt::DisplayRole,Qt::BackgroundRole});
            delete process;
        }
    );
    process->start(QCoreApplication::arguments().at(0),{"--test",test->name});
    test->result = Result::Running;
    emit dataChanged(ni,ni,{Qt::DisplayRole,Qt::ForegroundRole});
}


void UnitModel::runAll(
)
{
    for (int i = 0;i < _tests.size();i++)
    {
        run(createIndex(i,NameColumn));
    }
}
}
