#ifndef WIDGET_DIALOG_ORPHANFILES_H
#define WIDGET_DIALOG_ORPHANFILES_H
#include <QDialog>
#include "Model.h"
class QHBoxLayout;
class QListView;
class QStringListModel;
namespace Widget {
namespace Dialog {




/*!
 * This is a Qt dialog class. It provides a dialog for the user to view all
 * orphaned block files and actions to remove them.
 */
class OrphanFiles:
    public QDialog
{
    Q_OBJECT
    const Model::Project* _projectModel {nullptr};
    QListView* _listView {nullptr};
    QPushButton* _closeButton {nullptr};
    QPushButton* _gitRemoveAllButton {nullptr};
    QPushButton* _refreshButton {nullptr};
    QPushButton* _removeAllButton {nullptr};
    QStringListModel* _model {nullptr};
    QHBoxLayout* _buttonsLayout {nullptr};


    /*!
     * Constructs this new widget with the given project model, parent, and
     * flags.
     * 
     * The given project model must be valid and cannot be destroyed during the
     * lifetime of this widget.
     *
     * @param projectModel
     *        The project model.
     *
     * @param parent
     *        The parent.
     *
     * @param flags
     *        The flags.
     */
    public:
    OrphanFiles(
        const Model::Project* projectModel
        ,QWidget* parent = nullptr
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
     * Called when this widget's "git remove all" button is clicked.
     */
    private slots:
    void gitRemoveAll(
    );


    /*!
     * Called when this widget's project model's destroyed signal is emitted.
     */
    private slots:
    void onProjectModelDestroyed(
        QObject* object
    );


    /*!
     * Called when this widget's refresh button is clicked.
     */
    private slots:
    void refresh(
    );


    /*!
     * Called when this widget's "remove all" button is clicked.
     */
    private slots:
    void removeAll(
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
     * This widget's close button.
     */
    private:
    QPushButton* closeButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "git remove all" button.
     */
    private:
    QPushButton* gitRemoveAllButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's list view.
     */
    private:
    QListView* listView(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's model.
     */
    private:
    QStringListModel* model(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's refresh button.
     */
    private:
    QPushButton* refreshButton(
    );


    /*!
     * Removes all orphan block files using this widget's Qt string list model
     * as a source for the file paths of orphaned block files.
     * 
     * The given git flag is passed onto the project stream's "remove orphan
     * files" method.
     *
     * @param git
     *        The git flag.
     */
    private:
    void remove(
        bool git
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "remove all" button.
     */
    private:
    QPushButton* removeAllButton(
    );


    /*!
     * Restores the geometry and state of this widget using the Qt settings
     * system.
     */
    private:
    void restoreGS(
    );


    /*!
     * Saves the geometry and state of this widget using the Qt settings system.
     */
    private:
    void saveGS(
    ) const;
};
}
}


#endif
