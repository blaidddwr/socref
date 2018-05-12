#include "cppqt_gui_typeselection.h"
#include <QMenu>
#include "cppqt_blockfactory.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"



using namespace CppQt::Gui;






TypeSelection::TypeSelection(AbstractBlock* block, QWidget* parent):
   QPushButton(parent)
{
   setMenu(buildRootMenu(block));
}






QString TypeSelection::value() const
{
   return _value;
}






void TypeSelection::setValue(const QString& value)
{
   _value = value;
   setText(_value.replace("&","&&"));
}






void TypeSelection::selectionTriggered(int index)
{
   setValue(_types.at(index));
}






QMenu* TypeSelection::buildRootMenu(AbstractBlock* block)
{
   QMenu* ret {new QMenu(this)};
   while ( block )
   {
      const QList<TypeList*> list {block->makeListOfType<TypeList>(BlockFactory::TypeListType)};
      for (auto typeList: list)
      {
         ret->addMenu(buildMenu(typeList));
      }
      block = block->parent();
   }
   return ret;
}






QMenu* TypeSelection::buildMenu(TypeList* block)
{
   QMenu* ret {new QMenu(block->name(),this)};
   for (auto child: block->list())
   {
      if ( TypeList* valid = child->cast<TypeList>(BlockFactory::TypeListType) )
      {
         ret->addMenu(buildMenu(valid));
      }
      else if ( Type* valid = child->cast<Type>(BlockFactory::TypeType) ) addType(ret,valid);
   }
   return ret;
}






void TypeSelection::addType(QMenu* menu, Type* type)
{
   int index {_types.size()};
   _types.append(type->cppType());
   QAction* action {new QAction(type->cppType().replace("&","&&"),this)};
   connect(action,&QAction::triggered,[this,index]{ selectionTriggered(index); });
   menu->addAction(action);
}
