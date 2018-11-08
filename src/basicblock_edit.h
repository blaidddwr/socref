#ifndef BASICBLOCK_EDIT_H
#define BASICBLOCK_EDIT_H
#include "basicblock.h"
#include "gui_abstractedit.h"



class QLayout;
class QCheckBox;
//



/*!
 */
class BasicBlock::Edit : public Gui::AbstractEdit
{
   Q_OBJECT
public:
   Edit(BasicBlock* block);
protected:
   virtual QLayout* layout() override final;
   virtual void apply();
private:
   static QString checkId(const QDomElement& element);
private:
   QLayout* setupForm(const QDomElement& element);
   QLayout* setupLayout(const QDomElement& element);
   void addWidgets(QLayout* layout, const QDomElement& element);
   void addCheckBoxes(QLayout* layout, const QDomElement& element);
   void addLineEdit(QLayout* layout, const QDomElement& element);
   void addTextEdit(QLayout* layout, const QDomElement& element);
   void addListEdit(QLayout* layout, const QDomElement& element);
   void addTypeEdit(QLayout* layout, const QDomElement& element);
   QCheckBox* setupCheckBox(const QString& id, const QString& title);
   void add(QLayout* layout, const QDomElement& element, QWidget* item);
   void add(QLayout* layout, const QDomElement& element, QLayout* item);
   QVariant check(const QString& id, quint32 type);
   /*!
    */
   BasicBlock* _block;
   /*!
    */
   QMap<QString,QWidget*> _widgets;
};



#endif
