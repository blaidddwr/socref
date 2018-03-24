#include "cppqt_edit_enumeration.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <exception.h>
#include "cppqt_enumeration.h"



using namespace CppQt::Edit;






Enumeration::Enumeration(AbstractBlock* block, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Enumeration*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   saveSettings("cppqt.edit.enumeration.geometry");
}






QLayout* Enumeration::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addClass(ret);
   return ret;
}






bool Enumeration::apply()
{
   if ( !Base::apply() ) return false;
   _block->setClass(_classBox->isChecked());
   return true;
}






void Enumeration::addClass(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupClass());
}






QCheckBox* Enumeration::setupClass()
{
   _classBox = new QCheckBox(tr("Class"));
   _classBox->setChecked(_block->isClass());
   return _classBox;
}
