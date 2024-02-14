#ifndef WIDGET_WINDOW_MAIN_H
#define WIDGET_WINDOW_MAIN_H
#include <QMainWindow>
#include "Model.h"
#include "Widget.h"
namespace Widget {
namespace Window {




/*!
 * This is a Qt main window class. It is the main window for this application.
 */
class Main:
    public QMainWindow
{
    Q_OBJECT
    Model::Project* _projectModel {nullptr};
    QAction* _aboutAction {nullptr};
    QAction* _aboutQtAction {nullptr};
    QAction* _buildAction {nullptr};
    QAction* _closeAction {nullptr};
    QAction* _importAction {nullptr};
    QAction* _exitAction {nullptr};
    QAction* _exportAction {nullptr};
    QAction* _makeAction {nullptr};
    QAction* _openAction {nullptr};
    QAction* _orphanFilesAction {nullptr};
    QAction* _parseAction {nullptr};
    QAction* _propertiesAction {nullptr};
    QAction* _saveAction {nullptr};
    QAction* _saveAsAction {nullptr};
    QMenu* _codeMenu {nullptr};
    QMenu* _editMenu {nullptr};
    QMenu* _fileMenu {nullptr};
    QMenu* _helpMenu {nullptr};
    QMenu* _newMenu {nullptr};
    QMenu* _selectionMenu {nullptr};
    QToolBar* _codeToolBar {nullptr};
    QToolBar* _editToolBar {nullptr};
    QToolBar* _fileToolBar {nullptr};
    Widget::Project* _projectWidget {nullptr};


    public:
    Main(
        QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


    public:
    virtual void closeEvent(
        QCloseEvent* event
    ) override final;


    /*!
     * Called when this widget's about action is triggered.
     */
    private slots:
    void about(
    );


    /*!
     * Called when this widget's build action is triggered.
     */
    private slots:
    void build(
    );


    /*!
     * Called when this widget's close action is triggered.
     */
    private slots:
    void close(
    );


    /*!
     * Called when this widget's export action is triggered.
     */
    private slots:
    void exportProject(
    );


    /*!
     * Called when this widget's import action is triggered.
     */
    private slots:
    void import(
    );


    /*!
     * Called when this widget's make action is triggered.
     */
    private slots:
    void make(
    );


    /*!
     * Called when this widget's new action is triggered with the given language
     * index.
     *
     * @param index
     *        The language index.
     */
    private slots:
    void newProject(
        int index
    );


    /*!
     * Called when this instance's project model's modified changed signal is
     * emitted.
     */
    private slots:
    void onProjectModelModifiedChanged(
        bool value
    );


    /*!
     * Called when this instance's project model's name changed signal is
     * emitted.
     */
    private slots:
    void onProjectModelNameChanged(
    );


    /*!
     * Called when this widget's open action is triggered.
     */
    private slots:
    void open(
    );


    /*!
     * Called when this widget's orphan files action is triggered.
     */
    private slots:
    void orphanFiles(
    );


    /*!
     * Called when this widget's parse action is triggered.
     */
    private slots:
    void parse(
    );


    /*!
     * Called when this widget's properties action is triggered.
     */
    private slots:
    void properties(
    );


    /*!
     * Called when this widget's save action is triggered.
     */
    private slots:
    bool save(
    );


    /*!
     * Called when this widget's "save as" action is triggered.
     */
    private slots:
    bool saveAs(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's about action.
     */
    private:
    QAction* aboutAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's about Qt action.
     */
    private:
    QAction* aboutQtAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's build action.
     */
    private:
    QAction* buildAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's code menu.
     */
    private:
    QMenu* codeMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's code tool bar.
     */
    private:
    QToolBar* codeToolBar(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's edit menu.
     */
    private:
    QMenu* editMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's edit tool bar.
     */
    private:
    QToolBar* editToolBar(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's exit action.
     */
    private:
    QAction* exitAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's export action.
     */
    private:
    QAction* exportAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's file menu.
     */
    private:
    QMenu* fileMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's file tool bar.
     */
    private:
    QToolBar* fileToolBar(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's close action.
     */
    private:
    QAction* closeAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's help menu.
     */
    private:
    QMenu* helpMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's import action.
     */
    private:
    QAction* importAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's make action.
     */
    private:
    QAction* makeAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's new menu.
     */
    private:
    QMenu* newMenu(
    );


    /*!
     * Determines if it is OK to close this window's project model.
     * 
     * If this window's project model has unsaved changes then the user is asked
     * what to do about it. The user's options are save, discard, or cancel.
     *
     * @return
     * True if it is OK to close or false otherwise.
     */
    private:
    bool okToClose(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's open action.
     */
    private:
    QAction* openAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "orphan files" action.
     */
    private:
    QAction* orphanFilesAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's parse action.
     */
    private:
    QAction* parseAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's project widget.
     */
    private:
    Widget::Project* projectWidget(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's properties action.
     */
    private:
    QAction* propertiesAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's save action.
     */
    private:
    QAction* saveAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "save as" action.
     */
    private:
    QAction* saveAsAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's selection menu.
     */
    private:
    QMenu* selectionMenu(
    );


    /*!
     * Sets this widget's project model to the given model.
     *
     * @param model
     *        The model.
     */
    private:
    void setProjectModel(
        Model::Project* model
    );


    /*!
     * Updates this widget's actions.
     */
    private:
    void updateActions(
    );


    /*!
     * Updates this widget's window title.
     */
    private:
    void updateTitle(
    );
};
}
}


#endif
