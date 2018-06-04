#include "cppqt_type.h"
#include <QRegularExpression>
#include "cppqt_type_view.h"
#include "cppqt_type_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Type::_fields {"type"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Type::type() const
{
   return BlockFactory::TypeType;
}






/*!
 * Implements the interface that returns a reference to this block's factory which 
 * produces all block types for this project type. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Type::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Type::name() const
{
   return _type;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the static qt icon _ret_ is null then lead this block's icon. 
 *
 * 2. Return _ret_. 
 */
QIcon Type::icon() const
{
   // 1
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/type.svg");

   // 2
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Type::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Type::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns the number of fields this block contains. 
 *
 * @return The number of fields this object contains. 
 */
int Type::fieldSize() const
{
   return Field::Total;
}






/*!
 * Implements the interface that returns the field type for the given field index 
 * of this block. 
 *
 * @param index Index of the field whose field type is returned. 
 *
 * @return Field type of the given field index of this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given field index return its type. If the given index is not 
 *    defined then throw an exception. 
 */
AbstractBlock::Field Type::fieldType(int index) const
{
   // 1
   switch (index)
   {
   case Field::TypeName: return AbstractBlock::Field::String;
   default:
      {
         Exception::OutOfRange e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Given block field index %1 is out of range (%2 max).")
                      .arg(index)
                      .arg(fieldSize() - 1));
         throw e;
      }
   }
}






/*!
 * Implements the interface that returns the value of the field with the given 
 * index for this block. 
 *
 * @param index Index of the field whose value is returned. 
 *
 * @return Value of the field with the given index for this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given field index return its value. If the given index is not 
 *    defined then throw an exception. 
 */
QVariant Type::field(int index) const
{
   // 1
   switch (index)
   {
   case Field::TypeName: return _type;
   default:
      {
         Exception::OutOfRange e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Given block field index %1 is out of range (%2 max).")
                      .arg(index)
                      .arg(fieldSize() - 1));
         throw e;
      }
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Type::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Tests if the given string is a valid C++ type, returning true if it is valid. 
 * The only validation that is done is checking proper syntax since checking actual 
 * names is beyond the scope of this program. 
 *
 * @param value The string that is checked for valid C++ type syntax. 
 *
 * @return True if the given string has valid syntax for a C++ type or false 
 *         otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given string does not match the basic syntax of a C++ type then return 
 *    false. 
 *
 * 2. Create a boolean _ret_ set to true and find all template argument matches 
 *    within the given string saving matches to _matches_. 
 *
 * 3. Iterate through all _matches_. If any match is not a valid C++ template 
 *    argument then set _ret_ to false and stop iterating through all matches. 
 *
 * 4. Return _ret_. 
 */
bool Type::isValidTypeString(const QString& value)
{
   // 1
   if ( !QRegExp("\\s*(const\\s+)?((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*(<(.*)>)?)+(\\s*\\*(\\s*const)?)*\\s*&{0,2}\\s*").exactMatch(value) )
   {
      return false;
   }

   // 2
   bool ret {true};
   QRegularExpression regExp("<(.*)>");
   QRegularExpressionMatchIterator matches {regExp.globalMatch(value)};

   // 3
   while ( matches.hasNext() )
   {
      QRegularExpressionMatch match {matches.next()};
      if ( !isValidTemplateArgument(match.captured(1)) )
      {
         ret = false;
         break;
      }
   }

   // 4
   return ret;
}






/*!
 * Tests if the given string is a valid C++ template argument for a C++ type, 
 * returning true if it is valid. The only validation that is done is checking 
 * proper syntax. 
 *
 * @param value The string that is checked for valid C++ template argument syntax. 
 *
 * @return True if the given string has valid syntax for a C++ template argument or 
 *         false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize _ret_ to true, _arguments_ as the broken list of all arguments 
 *    within the given string, nd _regexp_ as the regular expression engine for 
 *    checking syntax. 
 *
 * 2. Iterate through all arguments in _arguments_. If an argument fails its syntax 
 *    check then set _ret_ to false and break out of the iteration. If the argument 
 *    is valid and itself contains another template argument then recursively call 
 *    this method to check it is also valid, setting _ret_ to false and breaking 
 *    from iteration if it is not valid. 
 *
 * 3. Return _ret_. 
 */
bool Type::isValidTemplateArgument(const QString& value)
{
   // 1
   bool ret {true};
   QStringList arguments {value.split(',')};
   QRegExp regexp("\\s*((((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?)|([0-9]+(\\.[0-9]+)?))\\s*");

   // 2
   for (auto arg : arguments)
   {
      if ( regexp.exactMatch(arg) )
      {
         if ( arg.contains("<") )
         {
            int begin {arg.indexOf('<')};
            int end {arg.lastIndexOf('>')};
            if ( !isValidTemplateArgument(arg.mid(begin+1,end-begin-1)) )
            {
               ret = false;
               break;
            }
         }
      }
      else
      {
         ret = false;
         break;
      }
   }

   // 3
   return ret;
}






/*!
 * Constructs a new type block with the given type value. 
 *
 * @param type The type value that this new type block's type name field is set to. 
 */
Type::Type(const QString& type):
   _type(type)
{
   checkType(_type);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Type::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Type);
}






/*!
 * Implements the interface that returns the current data version for this block 
 * type. 
 *
 * @return Current data version. 
 */
int Type::version() const
{
   return 0;
}






/*!
 * Implements the interface that returns the tag name for the field with the given 
 * index for this block. 
 *
 * @param index Index of the field whose tag name is returned. 
 *
 * @return Tag name for the field with the given index for this block. 
 */
QString Type::fieldTag(int index) const
{
   return _fields.at(index);
}






/*!
 * Implements the interface that returns the index of the field that has the given 
 * tag name for this block. 
 *
 * @param name Tag name of the field whose index is returned. 
 *
 * @return Index of the field with the given tag name or -1 if no field exists with 
 *         that tag name. 
 */
int Type::fieldIndexOf(const QString& name) const
{
   return _fields.indexOf(name);
}






/*!
 * Implements the interface that is called when the field with the given index for 
 * this block has been modified. 
 *
 * @param index Index of the field which has just been modified. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the field index given notify the changes to this block. 
 */
void Type::fieldModified(int index)
{
   // 1
   switch (index)
   {
   case Field::TypeName:
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
      break;
   }
}






/*!
 * Implements the interface that quietly sets the value of the field with the given 
 * index to the new given value. 
 *
 * @param index Index of the field whose value is set to the new given value. 
 *
 * @param value New value that the field with the given index is set to. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given field index set its value. If the given index is not 
 *    defined then do nothing. 
 */
void Type::quietlySetField(int index, const QVariant& value)
{
   // 1
   switch (index)
   {
   case Field::TypeName:
      setType(value.toString());
      break;
   }
}






/*!
 * Makes sure the given string is a valid C++ type. If it is not valid then an 
 * exception is thrown. 
 *
 * @param value The string whose syntax is checked. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given string is not a valid C++ type then throw an exception. 
 */
void Type::checkType(const QString& value)
{
   // 1
   if ( !isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given string '%1' is not a valid C++ type.").arg(value));
      throw e;
   }
}






/*!
 * Sets this block's type name field to the new value. If the given value is not a 
 * valid C++ type then an exception is thrown. 
 *
 * @param value The new value for this block's type name field. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Verify the given value is a valid C++ type and then set this block's type 
 *    name field to the new given value. 
 */
void Type::setType(const QString& value)
{
   // 1
   checkType(value);
   _type = value;
}
