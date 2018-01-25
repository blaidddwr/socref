#include "cppqt_gui_typecombobox.h"
#include "abstractblock.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace std;
using namespace CppQt::Gui;






TypeComboBox::TypeComboBox(AbstractBlock* block, QWidget* parent):
   QComboBox(parent)
{
   findNamespaces(block);
   buildComboList();
}






TypeComboBox& TypeComboBox::setCurrentIndex(const QString& text)
{
   QComboBox::setCurrentIndex(findText(text));
   return *this;
}






void TypeComboBox::comboActivated(int index)
{
   //TODO Open TypeDialog and add to appropriate list
}






void TypeComboBox::findNamespaces(AbstractBlock* block)
{
   while ( !(_local = qobject_cast<Namespace*>(block)) )
   {
      block = block->parent();
      if ( !block )
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Root block reached without finding a single namespace."));
         throw e;
      }
   }
   while ( block->parent() )
   {
      block = block->parent();
   }
   _global = qobject_cast<Namespace*>(block);
   if ( !_global )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not a namespace."));
      throw e;
   }
}






void TypeComboBox::buildComboList()
{
   QStringList types {_local->types()};
   if ( _local != _global )
   {
      types.append(_global->types());
   }
   sort(types.begin(),types.end());
   clear();
   for (auto type : qAsConst(types))
   {
      addItem(type);
   }
   _globalIndex = -2;
   _localIndex = count();
   addItem(tr("+ Add Local..."));
   if ( _local != _global )
   {
      _globalIndex = count();
      addItem(tr("+ Add Global..."));
   }
}
