#include "cppqt_edit_class.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <exception.h>
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
   saveSettings("cppqt.edit.class.geometry");
}






QLayout* Class::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   addProperties(ret);
   addTypeButtons(ret);
   return ret;
}






void Class::apply()
{
   Namespace::apply();
   _block->setQtObject(_qtObjectBox->isChecked());
}






void Class::addProperties(QFormLayout* layout)
{
   setupObject();
   layout->addRow(new QLabel(tr("Properties:")),_qtObjectBox);
}






void Class::setupObject()
{
   bool isTemplate {_block->hasAnyTemplates()};
   _qtObjectBox = new QCheckBox(tr("Qt Object"));
   _qtObjectBox->setChecked( ( _block->isQtObject() || _block->hasSignalsOrSlots() )
                             && !isTemplate );
   _qtObjectBox->setDisabled( _block->hasSignalsOrSlots() || isTemplate );
}
