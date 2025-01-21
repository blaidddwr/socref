#ifndef TEST_UNITMODEL_H
#define TEST_UNITMODEL_H
#include <QAbstractTableModel>
namespace Test {




/*!
 * This is a model class. It represents all unit tests for this application.
 * 
 * This model also provides methods for running any or all unit tests. Unit
 * tests are ran outside of the main GUI thread.
 */
class UnitModel:
    public QAbstractTableModel
{
    Q_OBJECT
    struct UnitTest
    {
        enum class Result
        {
            Failed
            ,None
            ,Passed
            ,Running
        };
        QString name;
        Result result;
        QString output;
        QString error;
    };
    QList<UnitTest> _tests;


    /*!
     * This enumerates all columns in this model.
     */
    public:
    enum Column
    {
        NameColumn
        ,ResultColumn
        ,ColumnTotal
    };


    public:
    UnitModel(
        QObject* parent = nullptr
    );


    public:
    virtual int columnCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


    public:
    virtual QVariant data(
        const QModelIndex& index
        ,int role = Qt::DisplayRole
    ) const override final;


    /*!
     * Returns the standard error from the last run of the unit test in this
     * model at the given index.
     * 
     * The given index must be valid.
     * 
     * If the unit test was never ran then this will return an empty string.
     *
     * @param index
     *        The index.
     */
    public:
    const QString& error(
        const QModelIndex& index
    ) const;


    public:
    virtual QVariant headerData(
        int section
        ,Qt::Orientation orientation
        ,int role = Qt::DisplayRole
    ) const override final;


    /*!
     * Returns the standard output from the last run of the unit test in this
     * model at the given index.
     * 
     * The given index must be valid.
     * 
     * If the unit test was never ran then this will return an empty string.
     *
     * @param index
     *        The index.
     */
    public:
    const QString& output(
        const QModelIndex& index
    ) const;


    public:
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Starts the unit test in this model at the given index.
     * 
     * The given index must be valid.
     * 
     * If the unit test is already running then this does nothing.
     *
     * @param index
     *        The index.
     */
    public:
    void run(
        const QModelIndex& index
    );


    /*!
     * Runs all unit tests by calling this model's run method on all valid
     * indexes.
     */
    public:
    void runAll(
    );
};
}


#endif
