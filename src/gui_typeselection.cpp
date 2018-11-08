#include "gui_typeselection.h"
#include <QJsonObject>
#include <QAction>
#include <QMenu>



using namespace Gui;
//






/*!
 * Constructs a new type selection widget with the given JSON object for menu 
 * generation and the optional parent. 
 *
 * @param object The root JSON object that is used to populate the menus and sub 
 *               menus of this new type selection widget. 
 *
 * @param parent Optional parent for this new widget. 
 */
TypeSelection::TypeSelection(const QJsonObject& object, QWidget* parent):
   QPushButton(parent)
{
   setMenu(setupMenu(object,""));
}






/*!
 * Returns the current variable type selected for this type selection widget. 
 *
 * @return Current C++ type selected for this type selection widget. 
 */
QString TypeSelection::value() const
{
   return _value;
}






/*!
 * Set the current variable type selected by this widget to the new value given. 
 *
 * @param value The new type value for this type selection. 
 */
void TypeSelection::setValue(QString value)
{
   // Set the current type value of this widget to the new one given and update the 
   // text on the button of this widget. 
   _value = value;
   setText(value.replace("&","&&"));
}






/*!
 * Called when the user chooses a type value from this selection's menu. 
 *
 * @param index The index of the type value chosen in the menu. 
 */
void TypeSelection::selectionTriggered(int index)
{
   setValue(_types.at(index));
}






/*!
 * Recursively builds and returns a type selection menu from the given JSON object. 
 * Any children JSON objects are recursively added as sub menus. 
 *
 * @param object The JSON object that is parsed to create a new type selection 
 *               menu. 
 *
 * @param key The key that is used to set the title of the returned menu. This 
 *            should be the key where this JSON object is stored in its parent JSON 
 *            object or empty if this is the root menu. 
 *
 * @return Type selection menu built from the given JSON object. 
 */
QMenu* TypeSelection::setupMenu(const QJsonObject& object, const QString& key)
{
   // Create a new menu, setting its title to the given key value. 
   QMenu* ret {new QMenu(key,this)};

   // Iterate through all children of the given JSON object. 
   for (auto i = object.begin(); i != object.end() ;++i)
   {
      // If the JSON value is am object then add a sub menu built by recursively calling 
      // this method, else if it is a null value then add it as an action to this menu 
      // using its key value as the type value. 
      switch (i->type())
      {
      case QJsonValue::Object:
         ret->addMenu(setupMenu(i->toObject(),i.key()));
         break;
      case QJsonValue::Null:
         addType(ret,i.key());
         break;
      default:
         break;
      }
   }

   // Return the new menu. 
   return ret;
}






/*!
 * Add a type selection action to the given menu with the given type value. 
 *
 * @param menu The menu which is added to. 
 *
 * @param value  
 */
void TypeSelection::addType(QMenu* menu, QString value)
{
   // Get the index where the new type will be added to this object's list of 
   // possible types and then add it. 
   int index {_types.size()};
   _types.append(value);

   // Create the action for selecting this new type value, replacing the ampersand to 
   // display correctly and connecting its triggered signal. 
   QAction* action {new QAction(value.replace("&","&&"),this)};
   connect(action,&QAction::triggered,[this,index]{ selectionTriggered(index); });

   // Add the new type selection action to the given menu. 
   menu->addAction(action);
}
