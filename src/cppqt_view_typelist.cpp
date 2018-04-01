#include "cppqt_view_typelist.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"
#include "cppqt_blockfactory.h"




using namespace CppQt::View;






TypeList::TypeList(const CppQt::TypeList* block, QWidget* parent):
   QLabel(parent),
   _block(block)
{
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   connect(_block,&CppQt::TypeList::bodyChanged,this,&TypeList::bodyChanged);
   setText(displayText());
}






QString TypeList::displayText()
{
   QString ret;
   const QList<Type*> list {_block->makeListOfType<Type>(BlockFactory::TypeType)};
   if ( !list.isEmpty() )
   {
      ret.append("<h3>Types</h3><p>");
      for (auto type: list) ret.append(type->name().replace("<","&lt;")).append("<br/>");
      ret.append("</p>");
   }
   return ret;
}






void TypeList::bodyChanged()
{
   setText(displayText());
}
