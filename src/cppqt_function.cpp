#include "cppqt_function.h"
#include <exception.h>
#include "cppqt_view_function.h"
#include "cppqt_edit_function.h"
#include "cppqt_variable.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_template.h"
#include "cppqt_access.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//



/*!
 */
const char* Function::_returnDescriptionTag {"return_description"};
/*!
 */
const char* Function::_defaultTag {"default"};
/*!
 */
const char* Function::_explicitTag {"explicit"};
/*!
 */
const char* Function::_virtualTag {"virtual"};
/*!
 */
const char* Function::_constTag {"const"};
/*!
 */
const char* Function::_noExceptTag {"noexcept"};
/*!
 */
const char* Function::_overrideTag {"override"};
/*!
 */
const char* Function::_finalTag {"final"};
/*!
 */
const char* Function::_abstractTag {"abstract"};
/*!
 */
const char* Function::_operationTag {"operation"};






/*!
 *
 * @param name  
 */
Function::Function(const QString& name):
   Variable(name)
{}






/*!
 *
 * @param returnType  
 *
 * @param name  
 */
Function::Function(const QString& returnType, const QString& name):
   Variable(returnType,name)
{}






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
   return unique_ptr<QWidget>(new View::Function(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Function::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Function(this));
}






/*!
 */
bool Function::isVoidReturn() const
{
   return variableType() == QString("void");
}






/*!
 */
bool Function::isMethod() const
{
   return isClassMember();
}






/*!
 */
bool Function::isPrivateMethod() const
{
   if ( !isClassMember() ) return false;
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
 */
QString Function::returnType() const
{
   return variableType();
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
 *
 * @param type  
 */
void Function::setReturnType(const QString& type)
{
   setVariableType(type);
}






/*!
 *
 * @param description  
 */
void Function::setReturnDescription(const QString& description)
{
   if ( _returnDescription != description )
   {
      _returnDescription = description;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param operations  
 */
void Function::setOperations(const QStringList& operations)
{
   if ( _operations != operations )
   {
      _operations = operations;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isDefault  
 */
void Function::setDefault(bool isDefault)
{
   if ( isDefault && !isClassMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as default when it is not a class member."));
      throw e;
   }
   if ( _default != isDefault )
   {
      _default = isDefault;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isExplicit  
 */
void Function::setExplicit(bool isExplicit)
{
   if ( _explicit != isExplicit )
   {
      _explicit = isExplicit;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isVirtual  
 */
void Function::setVirtual(bool isVirtual)
{
   if ( isVirtual && ( isStatic() || hasTemplates() || !isMethod() ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as virtual when it is static or has templates."));
      throw e;
   }
   if ( _virtual != isVirtual )
   {
      _virtual = isVirtual;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isConstExpr  
 */
void Function::setConstExpr(bool isConstExpr)
{
   if ( isConstExpr && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as constexpr when it is virtual."));
      throw e;
   }
   Variable::setConstExpr(isConstExpr);
}






/*!
 *
 * @param isStatic  
 */
void Function::setStatic(bool isStatic)
{
   if ( isStatic && _virtual )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as static when it is virtual."));
      throw e;
   }
   Variable::setStatic(isStatic);
}






/*!
 *
 * @param isConst  
 */
void Function::setConst(bool isConst)
{
   if ( isConst && !isMethod() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as const when it is not a class method."));
      throw e;
   }
   if ( _const != isConst )
   {
      _const = isConst;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isNoExcept  
 */
void Function::setNoExcept(bool isNoExcept)
{
   if ( _noExcept != isNoExcept )
   {
      _noExcept = isNoExcept;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isOverride  
 */
void Function::setOverride(bool isOverride)
{
   if ( isOverride && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as override when it is not virtual or it is abstract."));
      throw e;
   }
   if ( _override != isOverride )
   {
      _override = isOverride;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isFinal  
 */
void Function::setFinal(bool isFinal)
{
   if ( isFinal && ( !_virtual || _abstract ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as final when it is not virtual or it is abstract."));
      throw e;
   }
   if ( _final != isFinal )
   {
      _final = isFinal;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 *
 * @param isAbstract  
 */
void Function::setAbstract(bool isAbstract)
{
   if ( isAbstract && ( !_virtual || _override || _final ) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function as abstract when it is not virtual or it is override/final."));
      throw e;
   }
   if ( _abstract != isAbstract )
   {
      _abstract = isAbstract;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






/*!
 * Implements the interface that reads in the data for this block from the given 
 * XML element and version number. 
 *
 * @param element The XML element used to read in this blocks data. 
 *
 * @param version The version of the data stored in the XML. 
 */
void Function::readData(const QDomElement& element, int version)
{
   Variable::readData(element,version);
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
int Function::writeVersion() const
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
QDomElement Function::writeData(QDomDocument& document) const
{
   QDomElement ret {Variable::writeData(document)};
   if ( _default ) ret.appendChild(document.createElement(_defaultTag));
   if ( _explicit ) ret.appendChild(document.createElement(_explicitTag));
   if ( _virtual ) ret.appendChild(document.createElement(_virtualTag));
   if ( _const ) ret.appendChild(document.createElement(_constTag));
   if ( _noExcept ) ret.appendChild(document.createElement(_noExceptTag));
   if ( _override ) ret.appendChild(document.createElement(_overrideTag));
   if ( _final ) ret.appendChild(document.createElement(_finalTag));
   if ( _abstract ) ret.appendChild(document.createElement(_abstractTag));
   if ( !_returnDescription.isEmpty() )
   {
      ret.appendChild(makeElement(document,_returnDescriptionTag,_returnDescription));
   }
   for (auto operation : qAsConst(_operations))
   {
      ret.appendChild(makeElement(document,_operationTag,operation));
   }
   return ret;
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
 * Implements the interface that copies all data from the given block to this 
 * block, overwriting any data this block may already contain. 
 *
 * @param other The other block whose data will be copied. 
 */
void Function::copyDataFrom(const AbstractBlock* other)
{
   if ( const Function* valid = qobject_cast<const Function*>(other) )
   {
      Variable::copyDataFrom(other);
      _returnDescription = valid->_returnDescription;
      _default = valid->_default;
      _explicit = valid->_explicit;
      _virtual = valid->_virtual;
      _const = valid->_const;
      _noExcept = valid->_noExcept;
      _override = valid->_override;
      _final = valid->_final;
      _abstract = valid->_abstract;
      _operations = valid->_operations;
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
 */
QString Function::attributes() const
{
   QString ret;
   if ( _default ) ret.append("D");
   if ( _explicit ) ret.append("X");
   if ( _const ) ret.append("C");
   if ( _noExcept ) ret.append("N");
   if ( _override ) ret.append("O");
   if ( _final ) ret.append("F");
   if ( _abstract ) ret.append("0");
   if ( !ret.isEmpty() ) ret.prepend(" [").append("]");
   return ret;
}






/*!
 *
 * @param element  
 */
void Function::readVersion0(const QDomElement& element)
{
   _operations.clear();
   _returnDescription.clear();
   QList<QDomElement> operations;
   DomElementReader reader(element);
   _virtual = reader.attributeToInt(_virtualTag,false);
   _const = reader.attributeToInt(_constTag,false);
   _noExcept = reader.attributeToInt(_noExceptTag,false);
   _override = reader.attributeToInt(_overrideTag,false);
   _final = reader.attributeToInt(_finalTag,false);
   _abstract = reader.attributeToInt(_abstractTag,false);
   reader.set(_operationTag,&operations,false);
   reader.set(_returnDescriptionTag,&_returnDescription,false);
   reader.read();
   for (auto operation : qAsConst(operations)) _operations.append(operation.text());
}






/*!
 *
 * @param element  
 */
void Function::readVersion1(const QDomElement& element)
{
   _operations.clear();
   QList<QDomElement> operations;
   DomElementReader reader(element);
   reader.set(_defaultTag,&_default,false);
   reader.set(_explicitTag,&_explicit,false);
   reader.set(_virtualTag,&_virtual,false);
   reader.set(_constTag,&_const,false);
   reader.set(_noExceptTag,&_noExcept,false);
   reader.set(_overrideTag,&_override,false);
   reader.set(_finalTag,&_final,false);
   reader.set(_abstractTag,&_abstract,false);
   reader.set(_returnDescriptionTag,&_returnDescription,false);
   reader.set(_operationTag,&operations,false);
   reader.read();
   for (auto operation : qAsConst(operations)) _operations.append(operation.text());
}
