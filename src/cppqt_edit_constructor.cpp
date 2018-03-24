#include "cppqt_edit_constructor.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "cppqt_constructor.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Constructor::Constructor(AbstractBlock* block, QWidget* parent):
   Function(block,parent,false),
   _block(qobject_cast<CppQt::Constructor*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   saveSettings("cppqt.edit.constructor.geometry");
}






QLayout* Constructor::layout()
{
   QFormLayout* ret {new QFormLayout};
   Base::addDescription(ret);
   addOperations(ret);
   addProperties(ret);
   return ret;
}






bool Constructor::apply()
{
   if ( !Function::apply() ) return false;
   _block->setExplicit(_explicitBox->isChecked());
   return true;
}






void Constructor::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupProperties());
}






QLayout* Constructor::setupProperties()
{
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(setupNoExcept());
   ret->addWidget(setupExplicit());
   return ret;
}






QWidget*Constructor::setupExplicit()
{
   _explicitBox = new QCheckBox(tr("Explicit"));
   _explicitBox->setChecked(_block->isExplicit());
   return _explicitBox;
}
