#ifndef WIDGET_WINDOW_MAIN_H
#define WIDGET_WINDOW_MAIN_H
#include <QMainWindow>
namespace Widget {
namespace Window {




/*!
 * This is a Qt main window class. It is the main window for this application.
 */
class Main:
    public QMainWindow
{
    Q_OBJECT
    QAction* _aboutAction {nullptr};
    QAction* _aboutQtAction {nullptr};
    QAction* _buildAction {nullptr};
    QAction* _closeAction {nullptr};
    QAction* _exitAction {nullptr};
    QAction* _makeAction {nullptr};
    QAction* _openAction {nullptr};
    QAction* _parseAction {nullptr};
    QAction* _propertiesAction {nullptr};
    QAction* _saveAction {nullptr};
    QAction* _saveAsAction {nullptr};
    QMenu* _editMenu {nullptr};
    QMenu* _fileMenu {nullptr};
    QMenu* _helpMenu {nullptr};
    QMenu* _newMenu {nullptr};
    QToolBar* _editToolBar {nullptr};
    QToolBar* _fileToolBar {nullptr};


    public:
    Main(
        QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


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
    void new_(
        int index
    );


    /*!
     * Called when this widget's open action is triggered.
     */
    private slots:
    void open(
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
    void save(
    );


    /*!
     * Called when this widget's "save as" action is triggered.
     */
    private slots:
    void saveAs(
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
     * This widget's parse action.
     */
    private:
    QAction* parseAction(
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
};
}
}


#endif
