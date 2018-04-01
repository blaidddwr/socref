#include "cppqt_gui_typecombobox.h"
#include "cppqt_blockfactory.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"



using namespace CppQt::Gui;






TypeComboBox::TypeComboBox(AbstractBlock* block, QWidget* parent):
   QComboBox(parent)
{
   buildTypeList(block);
}






void TypeComboBox::buildTypeList(AbstractBlock* block)
{
   if ( block )
   {
      const QList<TypeList*> list {block->makeListOfType<TypeList>(BlockFactory::TypeListType)};
      if ( !list.isEmpty() )
      {
         for (auto typeList: list)
         {
            const QList<Type*> list {typeList->makeListOfType<Type>(BlockFactory::TypeType)};
            for (auto type: list) addItem(type->name());
         }
      }
      else buildTypeList(block->parent());
   }
}
