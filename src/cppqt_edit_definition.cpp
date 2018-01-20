#include <QHBoxLayout>
#include "cppqt_edit_definition.h"
#include "cppqt_definition.h"
#include "cppqt_type_gui_edit.h"
#include "exception.h"



using namespace std;
using namespace CppQt::Edit;






Definition::Definition(AbstractBlock* block, QWidget* parent):
   Gui::AbstractEdit(parent),
   _block(dynamic_cast<CppQt::Definition*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






void Definition::okClicked()
{
   if ( apply() )
   {
      finished();
   }
}






unique_ptr<QLayout> Definition::createForm()
{
   unique_ptr<QLayout> ret{new QHBoxLayout};
   _typeEdit = new Type::Gui::Edit(_block->getType());
   ret->addWidget(_typeEdit);
   return ret;
}






bool Definition::apply()
{
   if ( _typeEdit->isValid(true) )
   {
      _block->setType(_typeEdit->type());
      return true;
   }
   return false;
}
