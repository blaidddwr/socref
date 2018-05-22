#include "cppqt_variable.h"
#include <exception.h>
#include "cppqt_view_variable.h"
#include "cppqt_edit_variable.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_type.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Variable::_fields
{
   "constexpr"
   ,"static"
   ,"mutable"
   ,"type"
   ,"initializer"
};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Variable::type() const
{
   return BlockFactory::VariableType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Variable::name() const
{
   QString ret {Base::name()};
   if ( !_initializer.isEmpty() )
   {
      if ( isArgument() ) ret.append(" =");
      else ret.append(" {}");
   }
   ret.append(attributes());
   return ret;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Variable::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon static_;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/variable.svg");
      static_ = QIcon(":/icons/svariable.svg");
   }
   if ( _static ) return static_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Variable::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Variable::makeView() const
{
   return unique_ptr<QWidget>(new View::Variable(this));
}






/*!
 */
int Variable::fieldSize() const
{
   return Base::Field::Total + Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Variable::fieldType(int index) const
{
   switch (index)
   {
   case Field::ConstExpr:
   case Field::Static:
   case Field::Mutable: return AbstractBlock::Field::Boolean;
   case Field::Type:
   case Field::Initializer: return AbstractBlock::Field::String;
   default: return Base::fieldType(index - Field::Total);
   }
}






/*!
 *
 * @param index  
 */
QVariant Variable::field(int index) const
{
   switch (index)
   {
   case Field::ConstExpr: return _constExpr;
   case Field::Static: return _static;
   case Field::Mutable: return _mutable;
   case Field::Type: return _type;
   case Field::Initializer: return _initializer;
   default: return Base::field(index - Field::Total);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Variable::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Variable(this));
}






/*!
 *
 * @param name  
 */
Variable::Variable(const QString& name):
   Base(name)
{}






/*!
 *
 * @param type  
 *
 * @param name  
 */
Variable::Variable(const QString& type, const QString& name):
   Base(name),
   _type(type)
{
   checkTypeSyntax(type);
}






/*!
 */
bool Variable::isConstExpr() const
{
   return _constExpr;
}






/*!
 */
bool Variable::isStatic() const
{
   return _static;
}






/*!
 */
bool Variable::isMutable() const
{
   return _mutable;
}






/*!
 */
QString Variable::variableType() const
{
   return _type;
}






/*!
 */
bool Variable::hasInitializer() const
{
   return !_initializer.isEmpty();
}






/*!
 */
QString Variable::initializer() const
{
   return _initializer;
}






/*!
 */
bool Variable::isMember() const
{
   return parent()->type() == BlockFactory::AccessType;
}






/*!
 */
bool Variable::isArgument() const
{
   return qobject_cast<Function*>(parent());
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Variable::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Variable);
}






/*!
 * This interface returns the current version number of XML elements written for 
 * this block type. 
 *
 * @return Current version number. 
 */
int Variable::version() const
{
   return 0;
}






/*!
 *
 * @param index  
 */
QString Variable::fieldTag(int index) const
{
   if ( index >= Field::Total ) return Base::fieldTag(index - Field::Total);
   else return _fields.at(index);
}






/*!
 *
 * @param name  
 */
int Variable::fieldIndexOf(const QString& name) const
{
   int ret {_fields.indexOf(name)};
   if ( ret == -1 ) ret = Base::fieldIndexOf(name) + Field::Total;
   return ret;
}






/*!
 *
 * @param index  
 */
void Variable::fieldModified(int index)
{
   Q_UNUSED(index)
   if ( index < Field::Total )
   {
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
   else Base::fieldModified(index - Field::Total);
}






/*!
 *
 * @param index  
 *
 * @param value  
 */
void Variable::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::ConstExpr:
      setConstExpr(value.toBool());
      break;
   case Field::Static:
      setStatic(value.toBool());
      break;
   case Field::Mutable:
      setMutable(value.toBool());
      break;
   case Field::Type:
      setType(value.toString());
      break;
   case Field::Initializer:
      _initializer = value.toString();
      break;
   default:
      Base::quietlySetField(index - Field::Total,value);
   }
}






/*!
 *
 * @param value  
 */
void Variable::checkTypeSyntax(const QString& value)
{
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(value));
      throw e;
   }
}






/*!
 */
QString Variable::attributes() const
{
   QString ret;
   if ( _constExpr ) ret.append("X");
   if ( _mutable ) ret.append("M");
   if ( !ret.isEmpty() ) ret.prepend(" ");
   return ret;
}






/*!
 *
 * @param state  
 */
void Variable::setConstExpr(bool state)
{
   if ( state && isArgument() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as constant expression when it is a function argument."));
      throw e;
   }
   _constExpr = state;
}






/*!
 *
 * @param state  
 */
void Variable::setStatic(bool state)
{
   if ( state && !isMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is not a class member."));
      throw e;
   }
   _static = state;
}






/*!
 *
 * @param state  
 */
void Variable::setMutable(bool state)
{
   if ( state && !isMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as mutable when it is not a class member."));
      throw e;
   }
   _mutable = state;
}






/*!
 *
 * @param value  
 */
void Variable::setType(const QString& value)
{
   checkTypeSyntax(value);
   _type = value;
}
