#include "cppqt_enumeration.h"
#include <exception.h>
#include "cppqt_view_enumeration.h"
#include "cppqt_edit_enumeration.h"
#include "cppqt_blockfactory.h"
#include "cppqt_enumvalue.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Enumeration::_fields {"class"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Enumeration::type() const
{
   return BlockFactory::EnumerationType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Enumeration::name() const
{
   QString ret {Base::name()};
   if ( _class ) ret.append(" [C]");
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Enumeration::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumeration.svg");
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Enumeration::buildList() const
{
   return QList<int>{BlockFactory::EnumValueType};
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Enumeration::makeView() const
{
   return unique_ptr<QWidget>(new View::Enumeration(this));
}






/*!
 */
int Enumeration::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Enumeration::fieldType(int index) const
{
   switch (index)
   {
   case Field::Class: return AbstractBlock::Field::Boolean;
   default: return Base::fieldType(index);
   }
}






/*!
 *
 * @param index  
 */
QVariant Enumeration::field(int index) const
{
   switch (index)
   {
   case Field::Class: return _class;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Enumeration::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Enumeration(this));
}






/*!
 *
 * @param name  
 */
Enumeration::Enumeration(const QString& name):
   Base(name)
{}






/*!
 */
bool Enumeration::isClass() const
{
   return _class;
}






/*!
 */
QList<EnumValue*> Enumeration::values() const
{
   return makeListOfType<EnumValue>(BlockFactory::EnumValueType);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Enumeration::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Enumeration);
}






/*!
 *
 * @param index  
 */
void Enumeration::fieldModified(int index)
{
   switch (index)
   {
   case Field::Class:
      notifyModified();
      notifyNameModified();
      break;
   default:
      Base::fieldModified(index);
      break;
   }
}






/*!
 *
 * @param index  
 *
 * @param value  
 */
void Enumeration::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::Class:
      _class = value.toBool();
      break;
   default:
      Base::quietlySetField(index,value);
      break;
   }
}






/*!
 */
QStringList Enumeration::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }
   return ret;
}
