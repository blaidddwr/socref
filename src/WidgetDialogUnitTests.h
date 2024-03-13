#ifndef WIDGET_DIALOG_UNITTESTS_H
#define WIDGET_DIALOG_UNITTESTS_H
#include <QDialog>
#include "Test.h"
class QHBoxLayout;
class QSplitter;
class QTabWidget;
class QTableView;
class QTextEdit;
namespace Widget {
namespace Dialog {




/*!
 * This is a Qt dialog class. It exposes all possible unit tests for this
 * application to the user.
 */
class UnitTests:
    public QDialog
{
    Q_OBJECT
    QHBoxLayout* _buttonsLayout {nullptr};
    QPushButton* _closeButton {nullptr};
    QPushButton* _runAllButton {nullptr};
    QPushButton* _runButton {nullptr};
    QSplitter* _splitter {nullptr};
    QTabWidget* _tab {nullptr};
    QTableView* _tableView {nullptr};
    QTextEdit* _errorTextEdit {nullptr};
    QTextEdit* _outputTextEdit {nullptr};
    Test::UnitModel* _model {nullptr};


    public:
    UnitTests(
        QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


    public:
    virtual void closeEvent(
        QCloseEvent* event
    ) override final;


    public:
    virtual void hideEvent(
        QHideEvent* event
    ) override final;


    /*!
     * Called when this widget's table view's item selection model's current
     * index changed signal is emitted.
     */
    private slots:
    void onCurrentIndexChanged(
        const QModelIndex& current
        ,const QModelIndex& previous
    );


    /*!
     * Called when this widget's model's data changed signal is emitted.
     */
    private slots:
    void onDataChanged(
        const QModelIndex& topLeft
        ,const QModelIndex& bottomRight
        ,QList<int> roles
    );


    /*!
     * Called when this widget's run button is clicked.
     */
    private slots:
    void run(
    );


    /*!
     * Called when this widget's run all button is clicked.
     */
    private slots:
    void runAll(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's buttons layout.
     */
    private:
    QHBoxLayout* buttonsLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's close button widget.
     */
    private:
    QPushButton* closeButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's error text edit widget.
     */
    private:
    QTextEdit* errorTextEdit(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's model.
     */
    private:
    Test::UnitModel* model(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's output text edit widget.
     */
    private:
    QTextEdit* outputTextEdit(
    );


    /*!
     * Restores the geometry and state of this widget using the Qt settings
     * system.
     */
    private:
    void restoreGS(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's run all button widget.
     */
    private:
    QPushButton* runAllButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's run button widget.
     */
    private:
    QPushButton* runButton(
    );


    /*!
     * Saves the geometry and state of this widget using the Qt settings system.
     */
    private:
    void saveGS(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This widget's splitter widget.
     */
    private:
    QSplitter* splitter(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's tab widget.
     */
    private:
    QTabWidget* tab(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's table view widget.
     */
    private:
    QTableView* tableView(
    );
};
}
}


#endif
