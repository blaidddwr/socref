#ifndef WIDGET_DIALOG_PROJECT_H
#define WIDGET_DIALOG_PROJECT_H
#include <QDialog>
#include "Model.h"
class QFormLayout;
class QHBoxLayout;
class QLineEdit;
namespace Widget {
namespace Dialog {




/*!
 * This is a Qt dialog class. It provides a dialog for the user to view and edit
 * a project model's properties. These properties include its name and relative
 * code path.
 */
class Project:
    public QDialog
{
    Q_OBJECT
    Model::Project* _model;
    QFormLayout* _formLayout {nullptr};
    QHBoxLayout* _buttonsLayout {nullptr};
    QHBoxLayout* _codePathLayout {nullptr};
    QLineEdit* _codePathLine {nullptr};
    QLineEdit* _nameEdit {nullptr};
    QPushButton* _applyButton {nullptr};
    QPushButton* _cancelButton {nullptr};
    QPushButton* _codePathBrowseButton {nullptr};
    QPushButton* _okButton {nullptr};


    /*!
     * Constructs this new widget with the given model, parent, and flags.
     * 
     * The given model must be valid and cannot be destroyed during the lifetime
     * of this widget.
     *
     * @param model
     *        The model.
     *
     * @param parent
     *        The parent.
     *
     * @param flags
     *        The flags.
     */
    public:
    Project(
        Model::Project* model
        ,QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


    /*!
     * Called to apply the values in this widget's form to its project model.
     */
    private slots:
    void apply(
    );


    /*!
     * Called when this widget's code path browse button is clicked.
     */
    private slots:
    void codePathBrowse(
    );


    /*!
     * Called when this widget's project model's destroyed signal is emitted.
     */
    private slots:
    void onModelDestroyed(
        QObject* object
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's apply button.
     */
    private:
    QPushButton* applyButton(
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
     * This widget's cancel button.
     */
    private:
    QPushButton* cancelButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "code path browse" button.
     */
    private:
    QPushButton* codePathBrowseButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "code path" layout.
     */
    private:
    QHBoxLayout* codePathLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's "code path" line.
     */
    private:
    QLineEdit* codePathLine(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's form layout.
     */
    private:
    QFormLayout* formLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's name edit.
     */
    private:
    QLineEdit* nameEdit(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's OK button.
     */
    private:
    QPushButton* okButton(
    );
};
}
}


#endif
