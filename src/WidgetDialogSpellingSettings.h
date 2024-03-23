#ifndef WIDGET_DIALOG_SPELLINGSETTINGS_H
#define WIDGET_DIALOG_SPELLINGSETTINGS_H
#include <QDialog>
class QComboBox;
class QFormLayout;
class QHBoxLayout;
class QLineEdit;
namespace Widget {
namespace Dialog {




/*!
 * This is a Qt dialog class. It provides a dialog for the user to view and edit
 * Hunspell spelling settings for this application.
 */
class SpellingSettings:
    public QDialog
{
    Q_OBJECT
    QComboBox* _dictionaryComboBox {nullptr};
    QFormLayout* _formLayout {nullptr};
    QHBoxLayout* _buttonsLayout {nullptr};
    QHBoxLayout* _pathLayout {nullptr};
    QLineEdit* _pathLineEdit {nullptr};
    QPushButton* _applyButton {nullptr};
    QPushButton* _cancelButton {nullptr};
    QPushButton* _okButton {nullptr};
    QPushButton* _pathBrowseButton {nullptr};


    public:
    SpellingSettings(
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
     * Called to apply the values in this widget's form to the spelling settings
     * singleton class.
     */
    private slots:
    void apply(
    );


    /*!
     * Called when this widget's path browse button is clicked.
     */
    private slots:
    void pathBrowse(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's apply button widget.
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
     * This widget's cancel button widget.
     */
    private:
    QPushButton* cancelButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's dictionary combo box widget.
     */
    private:
    QComboBox* dictionaryComboBox(
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
     * This widget's OK button widget.
     */
    private:
    QPushButton* okButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's path browse button widget.
     */
    private:
    QPushButton* pathBrowseButton(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's path layout.
     */
    private:
    QHBoxLayout* pathLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's path line edit widget.
     */
    private:
    QLineEdit* pathLineEdit(
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


    /*!
     * Updates this widget's dictionary combo box widget with all valid Hunspell
     * dictionaries found in the path specified in this widget's path line edit
     * widget's text.
     * 
     * The first item in this widget's dictionary combo box is always none. None
     * represents selecting no dictionary and therefore disabling spell
     * checking.
     */
    private:
    void updateDictionaries(
    );
};
}
}


#endif
