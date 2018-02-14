#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <exception.h>
#include "cppqt_edit_class.h"
#include "cppqt_class.h"
#include "cppqt_common.h"



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
   addQtObject(ret);
   Base::addFields(ret);
   addTypeButtons(ret);
   return ret;
}






bool Class::apply()
{
   if ( !Namespace::apply() ) return false;
   _block->setQtObject(_qtObjectBox->isChecked());
   return true;
}






void Class::addQtObject(QFormLayout* layout)
{
   setupObject();
   layout->addRow(_qtObjectBox);
}






void Class::setupObject()
{
   _qtObjectBox = new QCheckBox(tr("Qt Object"));
   _qtObjectBox->setChecked(_block->isQtObject() || _block->hasSignalsOrSlots());
   _qtObjectBox->setDisabled(_block->hasSignalsOrSlots());
}
