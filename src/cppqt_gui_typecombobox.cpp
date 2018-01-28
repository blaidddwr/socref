#include "cppqt_gui_typecombobox.h"
#include "abstractblock.h"
#include "cppqt_namespace.h"
#include "exception.h"
#include "cppqt_gui_typelistdialog.h"



using namespace std;
using namespace CppQt::Gui;






TypeComboBox::TypeComboBox(AbstractBlock* block, QWidget* parent):
   QComboBox(parent)
{
   findNamespaces(block);
   buildComboList();
}






void TypeComboBox::setCurrentIndex(const QString& type)
{
   QComboBox::setCurrentIndex(findText(type));
}






QString TypeComboBox::value() const
{
   return itemText(currentIndex());
}






void TypeComboBox::comboActivated(int index)
{
   Namespace* edit {nullptr};
   if ( index == _localIndex )
   {
      edit = _local;
   }
   else if ( index == _globalIndex )
   {
      edit = _global;
   }
   if ( edit )
   {
      TypeListDialog dialog(edit);
      dialog.exec();
   }
}






void TypeComboBox::typeListChanged()
{
   QString current {value()};
   buildComboList();
   setCurrentIndex(current);
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
   if ( _local == _global )
   {
      _local = nullptr;
   }
   else
   {
      connect(_local,&Namespace::typesChanged,this,&TypeComboBox::typeListChanged);
   }
   connect(_global,&Namespace::typesChanged,this,&TypeComboBox::typeListChanged);
}






void TypeComboBox::buildComboList()
{
   QStringList types {_global->types()};
   if ( _local )
   {
      types.append(_local->types());
   }
   sort(types.begin(),types.end());
   clear();
   for (auto type : qAsConst(types))
   {
      addItem(type);
   }
   _globalIndex = count();
   addItem(tr("+ Edit Global..."));
   if ( _local )
   {
      _localIndex = count();
      addItem(tr("+ Edit Local..."));
   }
   else
   {
      _localIndex = -2;
   }
}
