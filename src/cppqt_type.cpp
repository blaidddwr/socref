#include "cppqt_type.h"
#include <QRegularExpression>
#include "cppqt_type_view.h"
#include "cppqt_type_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 * List of this block's field tag names that follow the same order as this block's 
 * enumeration of fields. 
 */
const QStringList Type::_fields {"type"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Type::type() const
{
   return BlockFactory::TypeType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& Type::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Type::name() const
{
   return _type;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Type::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/type.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Type::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Type::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Type::fieldSize() const
{
   // Use the field enumeration to return the total number of fields. 
   return Field::Total;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
AbstractBlock::Field Type::fieldType(int index) const
{
   // Based off the given field index return its type. 
   switch (index)
   {
   case Field::TypeName: return AbstractBlock::Field::String;

   // If the given index is unknown then throw an exception. 
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
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QVariant Type::field(int index) const
{
   // Based off the given field index return its value. 
   switch (index)
   {
   case Field::TypeName: return _type;

   // If the given index is unknown then throw an exception. 
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
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
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
 */
bool Type::isValidTypeString(const QString& value)
{
   // Make sure the given string matches basic syntax. 
   if ( !QRegExp("\\s*(const\\s+)?((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*(<(.*)>)?)+(\\s*\\*(\\s*const)?)*\\s*&{0,2}\\s*").exactMatch(value) )
   {
      return false;
   }

   // Find all template argument matches within the given string. 
   QRegularExpressionMatchIterator matches {QRegularExpression("<(.*)>").globalMatch(value)};

   // Iterate through all template argument matches. 
   while ( matches.hasNext() )
   {
      // If the current template argument match is not valid then return false. 
      QRegularExpressionMatch match {matches.next()};
      if ( !isValidTemplateArgument(match.captured(1)) ) return false;
   }

   // All syntax testing worked so return true. 
   return true;
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
 */
bool Type::isValidTemplateArgument(const QString& value)
{
   // Initialize the regular expression that will be used for basic syntax checking. 
   QRegExp regexp("\\s*((((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?)|([0-9]+(\\.[0-9]+)?))\\s*");

   // Split up all template arguments and iterate through them all. 
   QStringList arguments {value.split(',')};
   for (auto arg : arguments)
   {
      // Make sure the template argument matches basic syntax. 
      if ( regexp.exactMatch(arg) )
      {
         // Check to see if this template argument has additional arguments nested within 
         // it. 
         if ( arg.contains("<") )
         {
            // Recursively call this method to make sure nested template argument is valid, 
            // returning false if it is not. 
            int begin {arg.indexOf('<')};
            int end {arg.lastIndexOf('>')};
            if ( !isValidTemplateArgument(arg.mid(begin+1,end-begin-1)) ) return false;
         }
      }

      // Else the template argument failed basic syntax so return false. 
      else return false;
   }

   // All syntax testing worked so return true. 
   return true;
}






/*!
 * Returns the type string this this block. 
 *
 * @return Type string for this block. 
 */
QString Type::cppType() const
{
   return _type;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Type::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Type);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Type::version() const
{
   return 0;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Type::fieldTag(int index) const
{
   // Return this block's field tag at the given index. 
   return _fields.at(index);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param name See interface docs. 
 *
 * @return See interface docs. 
 */
int Type::fieldIndexOf(const QString& name) const
{
   // Return the index of this block's field tag that matches the given name. 
   return _fields.indexOf(name);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 */
void Type::fieldModified(int index)
{
   // Based off the field index given notify the changes to this block. If the given 
   // index is not defined then do nothing. 
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
 * Implements _AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @param value See interface docs. 
 */
void Type::quietlySetField(int index, const QVariant& value)
{
   // Based off the given field index set its value. If the given index is not 
   // defined then do nothing. 
   switch (index)
   {
   case Field::TypeName:
      setType(value.toString());
      break;
   }
}






/*!
 * Sets this block's type name field to the new value. If the given value is not a 
 * valid C++ type then an exception is thrown. 
 *
 * @param value The new value for this block's type name field. 
 */
void Type::setType(const QString& value)
{
   // Make sure the given value has valid type syntax. 
   if ( !isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given string '%1' is not a valid C++ type.").arg(value));
      throw e;
   }

   // Set this block's type value to the new one given. 
   _type = value;
}
