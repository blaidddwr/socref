#include "cppqt_variable.h"
#include <exception.h>
#include "cppqt_view_variable.h"
#include "cppqt_edit_variable.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_type.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const char* Variable::_constExprTag {"constexpr"};
/*!
 */
const char* Variable::_staticTag {"static"};
/*!
 */
const char* Variable::_mutableTag {"mutable"};
/*!
 */
const char* Variable::_typeTag {"type"};
/*!
 */
const char* Variable::_initializerTag {"initializer"};






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
      if ( isFunctionArgument() ) ret.append(" =");
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
 */
bool Variable::isConstExpr() const
{
   return _constExpr;
}






/*!
 *
 * @param isConstExpr  
 */
void Variable::setConstExpr(bool isConstExpr)
{
   if ( isConstExpr && isFunctionArgument() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is a function argument."));
      throw e;
   }
   if ( _constExpr != isConstExpr )
   {
      _constExpr = isConstExpr;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 */
bool Variable::isStatic() const
{
   return _static;
}






/*!
 *
 * @param isStatic  
 */
void Variable::setStatic(bool isStatic)
{
   if ( isStatic && !isClassMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is not a class member."));
      throw e;
   }
   if ( _static != isStatic )
   {
      _static = isStatic;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 */
bool Variable::isMutable() const
{
   return _mutable;
}






/*!
 *
 * @param isMutable  
 */
void Variable::setMutable(bool isMutable)
{
   if ( isMutable && !isClassMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as mutable when it is not a class member."));
      throw e;
   }
   if ( _mutable != isMutable )
   {
      _mutable = isMutable;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 */
void Variable::unnamed_function()
{}






/*!
 */
QString Variable::variableType() const
{
   return _type;
}






/*!
 *
 * @param type  
 */
void Variable::setVariableType(const QString& type)
{
   checkTypeSyntax(type);
   if ( _type != type )
   {
      _type = type;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
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
 *
 * @param initializer  
 */
void Variable::setInitializer(const QString& initializer)
{
   if ( _initializer != initializer )
   {
      _initializer = initializer;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 */
bool Variable::isClassMember() const
{
   if ( parent()->type() == BlockFactory::AccessType ) return true;
   else return false;
}






/*!
 */
bool Variable::isFunctionArgument() const
{
   return qobject_cast<Function*>(parent());
}






/*!
 * Implements the interface that reads in the data for this block from the given 
 * XML element and version number. 
 *
 * @param element The XML element used to read in this blocks data. 
 *
 * @param version The version of the data stored in the XML. 
 */
void Variable::readData(const QDomElement& element, int version)
{
   Base::readData(element,version);
   switch (version)
   {
   case 0:
      readVersion0(element);
      break;
   case 1:
      readVersion1(element);
      break;
   default:
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Unknown version number %1 given for reading block.").arg(version));
         throw e;
      }
   }
}






/*!
 * Implements the interface that returns the current version number of XML elements 
 * written for this block type. 
 *
 * @return Current version number. 
 */
int Variable::writeVersion() const
{
   return _version;
}






/*!
 * Implements the interface that returns a XML element containing the data for this 
 * block using the current version number. 
 *
 * @param document XML document to use for creating new elements. 
 *
 * @return XML element containing the data of this block. 
 */
QDomElement Variable::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _constExpr ) ret.appendChild(document.createElement(_constExprTag));
   if ( _static ) ret.appendChild(document.createElement(_staticTag));
   if ( _mutable ) ret.appendChild(document.createElement(_mutableTag));
   ret.appendChild(makeElement(document,_typeTag,_type));
   if ( !_initializer.isEmpty() )
   {
      ret.appendChild(makeElement(document,_initializerTag,_initializer));
   }
   return ret;
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
 * Implements the interface that copies all data from the given block to this 
 * block, overwriting any data this block may already contain. 
 *
 * @param other The other block whose data will be copied. 
 */
void Variable::copyDataFrom(const AbstractBlock* other)
{
   if ( const Variable* object_ = qobject_cast<const Variable*>(other) )
   {
      Base::copyDataFrom(other);
      _type = object_->_type;
      _constExpr = object_->_constExpr;
      _static = object_->_static;
      _mutable = object_->_mutable;
      _initializer = object_->_initializer;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
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
 * @param type  
 */
void Variable::checkTypeSyntax(const QString& type)
{
   if ( !Type::isValidTypeString(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
      throw e;
   }
}






/*!
 *
 * @param element  
 */
void Variable::readVersion0(const QDomElement& element)
{
   DomElementReader reader(element);
   _constExpr = reader.attributeToInt(_constExprTag,false);
   _static = reader.attributeToInt(_staticTag,false);
   _type = reader.attribute(_typeTag);
   _initializer = reader.attribute(_initializerTag,false);
}






/*!
 *
 * @param element  
 */
void Variable::readVersion1(const QDomElement& element)
{
   DomElementReader reader(element);
   reader.set(_constExprTag,&_constExpr,false);
   reader.set(_staticTag,&_static,false);
   reader.set(_mutableTag,&_mutable,false);
   reader.set(_typeTag,&_type);
   reader.set(_initializerTag,&_initializer,false);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
}
