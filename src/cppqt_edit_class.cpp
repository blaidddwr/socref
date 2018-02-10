#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <exception.h>
#include "cppqt_edit_class.h"
#include "cppqt_class.h"



using namespace CppQt::Edit;






Class::Class(AbstractBlock* block, QWidget* parent):
   Namespace(block,parent),
   _block(qobject_cast<CppQt::Class*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Class::layout()
{
   QFormLayout* ret {new QFormLayout};
   ret->addRow(setupTitle(tr("Qt")));
   addQtObject(ret);
   ret->addRow(setupTitle(tr("Basic Information")));
   Base::addFields(ret);
   ret->addRow(setupTitle(tr("Types")));
   addTypeButtons(ret);
   return ret;
}






void Class::applyClicked()
{
   Namespace::applyClicked();
   _block->setQtObject(_qtObjectBox->isChecked());
}






void Class::addQtObject(QFormLayout* layout)
{
   _qtObjectBox = new QCheckBox(tr("Qt Object"));
   _qtObjectBox->setChecked(_block->isQtObject());
   layout->addRow(_qtObjectBox);
}
