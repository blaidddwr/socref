#ifndef CPPQT_EDIT_NAMESPACE_H
#define CPPQT_EDIT_NAMESPACE_H
#include "gui_abstractedit.h"



class AbstractBlock;
class QLineEdit;
class QPlainTextEdit;



//@@
namespace CppQt {

class Namespace;

//@@
namespace Edit {

//@@
class Namespace : public Gui::AbstractEdit
{
   Q_OBJECT
public:
   Namespace(AbstractBlock* block, QWidget* parent = nullptr);
   virtual QLayout* createForm() override final;
protected slots:
   virtual void okClicked() override final;
   virtual void applyClicked() override final;
   virtual void cancelClicked() override final;
private:
   CppQt::Namespace* _block;
   QLineEdit* _nameEdit;
   QPlainTextEdit* _descriptionEdit;
};

}

}



#endif
