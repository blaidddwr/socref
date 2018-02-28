#include "cppqt_view_class.h"
#include <exception.h>
#include "cppqt_view_common.h"
#include "cppqt_class.h"
#include "cppqt_template.h"



using namespace CppQt::View;






Class::Class(const AbstractBlock* block, QWidget* parent):
   Namespace(block,true,parent),
   _block(qobject_cast<const CppQt::Class*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   connect(_block,&CppQt::Class::bodyChanged,this,&Class::bodyChanged);
   setText(displayText());
}






QString Class::displayText()
{
   return Namespace::displayText().append(displayTemplatesText()).append(displayQtObjectText());
}






QString Class::displayQtObjectText()
{
   return QString("<p><b>Qt Object</b> : ").append(_block->isQtObject()?"Yes":"No").append("</p>");
}






QString Class::displayTemplatesText()
{
   return View::displayTemplatesText(_block);
}






void Class::bodyChanged()
{
   setText(displayText());
}
