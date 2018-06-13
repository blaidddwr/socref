#include "cppqt_function.h"
#include <exception.h>
#include "cppqt_function_view.h"
#include "cppqt_function_edit.h"
#include "cppqt_variable.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_template.h"
#include "cppqt_access.h"
#include "cppqt_type.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const QStringList Function::_fields
{
   "default"
   ,"explicit"
   ,"virtual"
   ,"const"
   ,"constexpr"
   ,"static"
   ,"noexcept"
   ,"override"
   ,"final"
   ,"abstract"
   ,"type"
   ,"return_description"
   ,"operation"
};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Function::type() const
{
   return BlockFactory::FunctionType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Function::name() const
{
   QString ret;
   if ( hasTemplates() ) ret.append("<> ");
   return ret.append(fullName(!isVoidReturn(),Base::name()));
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Function::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   static QIcon static_;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/function.svg");
      virtual_ = QIcon(":/icons/virtual.svg");
      abstract = QIcon(":/icons/abstract.svg");
      static_ = QIcon(":/icons/static.svg");
      isLoaded = true;
   }
   if ( _abstract ) return abstract;
   else if ( _virtual ) return virtual_;
   else if ( isStatic() ) return static_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Function::buildList() const
{
   QList<int> ret;
   ret << BlockFactory::VariableType;
   if ( !_virtual ) ret << BlockFactory::TemplateType;
   return ret;
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Function::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 */
int Function::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Function::fieldType(int index) const
{
   switch (index)
   {
   case Field::Default:
   case Field::Explicit:
   case Field::Virtual:
   case Field::Const:
   case Field::ConstExpr:
   case Field::Static:
   case Field::NoExcept:
   case Field::Override:
   case Field::Final:
   case Field::Abstract: return AbstractBlock::Field::Boolean;
   case Field::ReturnType:
   case Field::ReturnDescription: return AbstractBlock::Field::String;
   case Field::Operations: return AbstractBlock::Field::StringList;
   default: return Base::fieldType(index);
   }
}






/*!
 *
 * @param index  
 */
QVariant Function::field(int index) const
{
   switch (index)
   {
   case Field::Default: return _default;
   case Field::Explicit: return _explicit;
   case Field::Virtual: return _virtual;
   case Field::Const: return _const;
   case Field::ConstExpr: return _constExpr;
   case Field::Static: return _static;
   case Field::NoExcept: return _noExcept;
   case Field::Override: return _override;
   case Field::Final: return _final;
   case Field::Abstract: return _abstract;
   case Field::ReturnType: return _returnType;
   case Field::ReturnDescription: return _returnDescription;
   case Field::Operations: return _operations;
   default: return Base::field(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Function::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 *
 * @param name  
 */
Function::Function(const QString& name):
   Base(name)
{}






/*!
 *
 * @param returnType  
 *
 * @param name  
 */
Function::Function(const QString& returnType, const QString& name):
   Base(name),
   _returnType(returnType)
{
   checkTypeSyntax(returnType);
}






/*!
 */
bool Function::isDefault() const
{
   return _default;
}






/*!
 */
bool Function::isExplicit() const
{
   return _explicit;
}






/*!
 */
bool Function::isVirtual() const
{
   return _virtual;
}






/*!
 */
bool Function::isConst() const
{
   return _const;
}






/*!
 */
bool Function::isConstExpr() const
{
   return _constExpr;
}






/*!
 */
bool Function::isStatic() const
{
   return _static;
}






/*!
 */
bool Function::isNoExcept() const
{
   return _noExcept;
}






/*!
 */
bool Function::isOverride() const
{
   return _override;
}






/*!
 */
bool Function::isFinal() const
{
   return _final;
}






/*!
 */
bool Function::isAbstract() const
{
   return _abstract;
}






/*!
 */
QString Function::returnType() const
{
   return _returnType;
}






/*!
 */
QString Function::returnDescription() const
{
   return _returnDescription;
}






/*!
 */
QStringList Function::operations() const
{
   return _operations;
}






/*!
 */
bool Function::isVoidReturn() const
{
   return _returnType == QString("void");
}






/*!
 */
bool Function::isMethod() const
{
   return parent()->type() == BlockFactory::AccessType;
}






/*!
 */
bool Function::isPrivateMethod() const
{
   if ( !isMethod() ) return false;
   return parent()->cast<Access>(BlockFactory::AccessType)->accessType() == Access::Type::Private;
}






/*!
 */
bool Function::hasAnyTemplates() const
{
   if ( hasTemplates() ) return true;
   if ( parent()->type() == BlockFactory::AccessType )
   {
      return parent()->parent()->cast<Class>(BlockFactory::ClassType)->hasAnyTemplates();
   }
   return false;
}






/*!
 */
bool Function::hasTemplates() const
{
   return containsType(BlockFactory::TemplateType);
}






/*!
 */
QList<Template*> Function::templates() const
{
   return makeListOfType<Template>(BlockFactory::TemplateType);
}






/*!
 */
QList<Variable*> Function::arguments() const
{
   return makeListOfType<Variable>(BlockFactory::VariableType);
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Function::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Function);
}






/*!
 *
 * @param index  
 */
void Function::fieldModified(int index)
{
   switch (index)
   {
   case Field::Default:
   case Field::Explicit:
   case Field::Virtual:
   case Field::Const:
   case Field::ConstExpr:
   case Field::Static:
   case Field::NoExcept:
   case Field::Override:
   case Field::Final:
   case Field::Abstract:
   case Field::ReturnType:
   case Field::ReturnDescription:
   case Field::Operations:
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
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
void Function::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::Default:
      setDefault(value.toBool());
      break;
   case Field::Explicit:
      setExplicit(value.toBool());
      break;
   case Field::Virtual:
      setVirtual(value.toBool());
      break;
   case Field::Const:
      setConst(value.toBool());
      break;
   case Field::ConstExpr:
      setConstExpr(value.toBool());
      break;
   case Field::Static:
      setStatic(value.toBool());
      break;
   case Field::NoExcept:
      _noExcept = value.toBool();
      break;
   case Field::Override:
      setOverride(value.toBool());
      break;
   case Field::Final:
      setFinal(value.toBool());
      break;
   case Field::Abstract:
      setAbstract(value.toBool());
      break;
   case Field::ReturnType:
      setReturnType(value.toString());
      break;
   case Field::ReturnDescription:
      _returnDescription = value.toString();
      break;
   case Field::Operations:
      _operations = value.toStringList();
      break;
   default: return Base::quietlySetField(index,value);
   }
}






/*!
 * Implements the interface that is called whenever a new child below this block 
 * has been added and keeps calling this interface on the next block parent until 
 * this returns false. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool Function::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that is called whenever an existing child below this 
 * block has been removed and keeps calling this interface on the next block parent 
 * until this returns false. 
 *
 * @param child Pointer to the child block that has been removed from its former 
 *              parent block. This object can be deleted right after this call. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool Function::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 */
QStringList Function::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Base::fields());
      ret.append(_fields);
   }
   return ret;
}






/*!
 *
 * @param hasReturn  
 *
 * @param name  
 */
QString Function::fullName(bool hasReturn, const QString& name) const
{
   QString ret;
   if ( hasReturn ) ret.append("... ");
   ret.append(name).append("(");
   const QList<Variable*> list {arguments()};
   ret.append(QString::number(list.size())).append(")").append(attributes());
   return ret;
}






/*!
 *
 * @param value  
 */
void Function::checkTypeSyntax(const QString& value)
{
   if ( !Type::isValidTypeString(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid return type '%1'.").arg(value));
      throw e;
   }
}






/*!
 */
QString Function::attributes() const
{
   QString ret;
   if ( _default ) ret.append("D");
   if ( _explicit ) ret.append("E");
   if ( _const ) ret.append("C");
   if ( _constExpr ) ret.append("X");
   if ( _noExcept ) ret.append("N");
   if ( _override ) ret.append("O");
   if ( _final ) ret.append("F");
   if ( _abstract ) ret.append("A");
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");
   return ret;
}






/*!
 *
 * @param state  
 */
void Function::setDefault(bool state)
{
   if ( state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as default when it is not a class method."));
      throw e;
   }
   _default = state;
}






/*!
 *
 * @param state  
 */
void Function::setExplicit(bool state)
{
   if ( state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as explicit when it is not a class method."));
      throw e;
   }
   _explicit = state;
}






/*!
 *
 * @param state  
 */
void Function::setVirtual(bool state)
{
   if ( state && ( isStatic() || hasTemplates() || !isMethod() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as virtual when it is static, has templates, or is not a class method."));
      throw e;
   }
   _virtual = state;
}






/*!
 *
 * @param state  
 */
void Function::setConst(bool state)
{
   if ( state && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as const when it is not a class method."));
      throw e;
   }
   _const = state;
}






/*!
 *
 * @param state  
 */
void Function::setConstExpr(bool state)
{
   if ( state && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as constexpr when it is virtual."));
      throw e;
   }
   _constExpr = state;
}






/*!
 *
 * @param state  
 */
void Function::setStatic(bool state)
{
   if ( state && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as static when it is virtual."));
      throw e;
   }
   _static = state;
}






/*!
 *
 * @param state  
 */
void Function::setOverride(bool state)
{
   if ( state && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as override when it is not virtual or it is abstract."));
      throw e;
   }
   _override = state;
}






/*!
 *
 * @param state  
 */
void Function::setFinal(bool state)
{
   if ( state && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as final when it is not virtual or it is abstract."));
      throw e;
   }
   _final = state;
}






/*!
 *
 * @param state  
 */
void Function::setAbstract(bool state)
{
   if ( state && ( !_virtual || _override || _final ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as abstract when it is not virtual or it is override/final."));
      throw e;
   }
   _abstract = state;
}






/*!
 *
 * @param value  
 */
void Function::setReturnType(const QString& value)
{
   checkTypeSyntax(value);
   _returnType = value;
}
