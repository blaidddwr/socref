#include "cppqt_gui_typeselection.h"
#include <QMenu>
#include "cppqt_blockfactory.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"



using namespace CppQt::Gui;
//






/*!
 * Constructs a new type selection widget with the given block for menu generation 
 * and the optional parent. 
 *
 * @param block The block that is used to generate the type selection menu based 
 *              off any type lists contained in any of its parents. 
 *
 * @param parent Optional parent for this new widget. 
 */
TypeSelection::TypeSelection(AbstractBlock* block, QWidget* parent):
   QPushButton(parent)
{
   setMenu(buildRootMenu(block));
}






/*!
 * Returns the current C++ type selected for this type selection widget. 
 *
 * @return Current C++ type selected for this type selection widget. 
 */
QString TypeSelection::value() const
{
   return _value;
}






/*!
 * Set the current C++ type selected by this widget to the new value given. 
 *
 * @param value The new type value for this type selection. 
 */
void TypeSelection::setValue(const QString& value)
{
   // Set the current type value of this widget to the new one given. 
   _value = value;

   // Set the display text of this button, replacing ampersand with double ampersand 
   // so it displays correctly. 
   setText(value.right(-1).replace("&","&&"));
}






/*!
 * Called when the user chooses a type value from this selection's menu. 
 *
 * @param index The index of the type value chosen in the menu. 
 */
void TypeSelection::selectionTriggered(int index)
{
   // Set the current C++ type of this selection to the value at the given index. 
   setValue(_types.at(index));
}






/*!
 * Builds and returns the type selection menu for this new widget from the given 
 * block. 
 *
 * @param block The block that is used to generate the returned type selection 
 *              menu. 
 *
 * @return The type selection menu for this new widget. 
 */
QMenu* TypeSelection::buildRootMenu(AbstractBlock* block)
{
   // Create a new menu. 
   QMenu* ret {new QMenu(this)};

   // Iterate up to the next block's parent until it is null. 
   while ( block )
   {
      // Iterate through all type list children blocks of the current parent block and 
      // build a sub menu for each one. 
      const QList<TypeList*> list {block->makeListOfType<TypeList>(BlockFactory::TypeListType)};
      for (auto typeList: list)
      {
         ret->addMenu(buildMenu(typeList));
      }

      // Move up to the next parent. 
      block = block->parent();
   }

   // Return the built type selection menu. 
   return ret;
}






/*!
 * Builds and returns a sub menu for the type selection menu from the given type 
 * list. Any children type lists are recursively added to the returned sub menu. 
 *
 * @param block The type list block used to generate the returned sub menu. 
 *
 * @return Sub menu for the type selection menu built from the given type list. 
 */
QMenu* TypeSelection::buildMenu(TypeList* block)
{
   // Create a new sub menu using the name of the given type list. 
   QMenu* ret {new QMenu(block->name(),this)};

   // Iterate through all children of the given type list block. 
   for (auto child: block->list())
   {
      // If the child block is a type list then add a sub menu built by recursively 
      // calling this method. 
      if ( TypeList* valid = child->cast<TypeList>(BlockFactory::TypeListType) )
      {
         ret->addMenu(buildMenu(valid));
      }

      // Else if the block is a simple type then add it. 
      else if ( Type* valid = child->cast<Type>(BlockFactory::TypeType) ) addType(ret,valid);
   }

   // Return the sub menu. 
   return ret;
}






/*!
 * Add a type selection action to the given menu with the given type block. 
 *
 * @param menu The menu which is added to. 
 *
 * @param type The type block used to generate the type selection added. 
 */
void TypeSelection::addType(QMenu* menu, Type* type)
{
   // Get the index where the new type will be added to this object's list of 
   // possible types and then add it. 
   int index {_types.size()};
   _types.append(type->cppType());

   // Create the action for selecting this new type value, replacing the ampersand to 
   // display correctly and connecting its triggered signal. 
   QAction* action {new QAction(type->cppType().replace("&","&&"),this)};
   connect(action,&QAction::triggered,[this,index]{ selectionTriggered(index); });

   // Add the new type selection action to the given menu. 
   menu->addAction(action);
}
