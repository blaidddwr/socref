#include "cppqt_type.h"
#include <QRegularExpression>
#include "cppqt_view_type.h"
#include "cppqt_edit_type.h"
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
 */
QIcon Type::icon() const
{
   QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/type.svg");
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
   return unique_ptr<QWidget>(new View::Type(this));
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
 */
AbstractBlock::Field Type::fieldType(int index) const
{
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
 */
QVariant Type::field(int index) const
{
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
   return unique_ptr<AbstractEdit>(new Edit::Type(this));
}






/*!
 *
 * @param value  
 */
bool Type::isValidTypeString(const QString& value)
{
   bool ret {false};
   if ( QRegExp("\\s*(const\\s+)?((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*(<(.*)>)?)+(\\s*\\*(\\s*const)?)*\\s*&{0,2}\\s*").exactMatch(value) )
   {
      ret = true;
      QRegularExpression regExp("<(.*)>");
      QRegularExpressionMatchIterator matches {regExp.globalMatch(value)};
      while ( matches.hasNext() )
      {
         QRegularExpressionMatch match {matches.next()};
         if ( !isValidTemplateArgument(match.captured(1)) )
         {
            ret = false;
            break;
         }
      }
   }
   return ret;
}






/*!
 *
 * @param value  
 */
bool Type::isValidTemplateArgument(const QString& value)
{
   bool ret {true};
   QStringList arguments {value.split(',')};
   for (auto arg : arguments)
   {
      if ( QRegExp("\\s*((((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?)|([0-9]+(\\.[0-9]+)?))\\s*").exactMatch(arg) )
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
   return ret;
}






/*!
 *
 * @param type  
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
 */
void Type::fieldModified(int index)
{
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
 */
void Type::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::TypeName:
      setType(value.toString());
      break;
   }
}






/*!
 *
 * @param value  
 */
void Type::checkType(const QString& value)
{
   if ( !isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given string '%1' is not a valid C++ type.").arg(value));
      throw e;
   }
}






/*!
 *
 * @param value  
 */
void Type::setType(const QString& value)
{
   checkType(value);
   _type = value;
}
