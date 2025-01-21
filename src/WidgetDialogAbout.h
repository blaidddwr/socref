#ifndef WIDGET_DIALOG_ABOUT_H
#define WIDGET_DIALOG_ABOUT_H
#include <QDialog>
class QHBoxLayout;
class QLabel;
namespace Widget {
namespace Dialog {




/*!
 * This is a Qt dialog class. It is a simple view informing the user about this
 * application.
 */
class About:
    public QDialog
{
    Q_OBJECT
    QHBoxLayout* _buttonsLayout {nullptr};
    QLabel* _bodyLabel {nullptr};
    QPushButton* _okButton {nullptr};


    public:
    About(
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
     * Getter method.
     *
     * @return
     * This widget's body label widget.
     */
    private:
    QLabel* bodyLabel(
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
     * This widget's OK push button widget.
     */
    private:
    QPushButton* okButton(
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
