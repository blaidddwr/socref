#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <QWidget>



//@@
namespace Gui {

//@@
class AbstractEdit : public QWidget
{
   Q_OBJECT
public:
   explicit AbstractEdit(QWidget* parent);
   //@@
   virtual QLayout* createForm() = 0;
   void initialize();
signals:
   //@@
   void finished();
protected slots:
   virtual void okClicked() = 0;
   virtual void applyClicked() = 0;
   virtual void cancelClicked() = 0;
private:
   QLayout* createButtons();
};



//@@
inline AbstractEdit::AbstractEdit(QWidget *parent): QWidget(parent) {}

}



#endif
