#include "cppqt_view_class.h"
#include "cppqt_view_common.h"
#include "cppqt_class.h"
#include "cppqt_template.h"
#include "cppqt_parent.h"



using namespace CppQt::View;






Class::Class(const CppQt::Class* block, QWidget* parent):
   Namespace(block,true,parent),
   _block(block)
{
   setText(displayText());
}






QString Class::displayText()
{
   return Namespace::displayText().append(displayTemplatesText())
                                  .append(displayParentText())
                                  .append(displayQtObjectText());
}






QString Class::displayQtObjectText()
{
   return QString("<p><b>Qt Object</b> : ").append(_block->isQtObject()?"Yes":"No").append("</p>");
}






QString Class::displayTemplatesText()
{
   return View::displayTemplatesText(_block);
}






QString Class::displayParentText()
{
   QString ret;
   const QList<Parent*> list {_block->parents()};
   if ( !list.isEmpty() )
   {
      ret.append("<h3>Inheritance</h3>");
      for (auto parent : list)
      {
         ret.append("<p><b>")
            .append(parent->accessName())
            .append("</b> ")
            .append(parent->className().replace("<","&lt;"))
            .append("</p>");
      }
   }
   return ret;
}
