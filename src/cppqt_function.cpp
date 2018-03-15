#include "cppqt_function.h"
#include <exception.h>
#include "cppqt_view_function.h"
#include "cppqt_edit_function.h"
#include "cppqt_gui_typedialog.h"
#include "cppqt_variable.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_template.h"
#include "cppqt_common.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Function::_returnDescriptionTag {"return_description"};
const char* Function::_virtualTag {"virtual"};
const char* Function::_constTag {"const"};
const char* Function::_noExceptTag {"noexcept"};
const char* Function::_overrideTag {"override"};
const char* Function::_finalTag {"final"};
const char* Function::_abstractTag {"abstract"};
const char* Function::_operationTag {"operation"};






Function::Function(const QString& name):
   Variable(name)
{}






Function::Function(const QString& returnType, const QString& name):
   Variable(returnType,name)
{}






QString Function::name() const
{
   QString ret {getTemplateName(this)};
   return ret.append(fullName(returnType(),Base::name()));
}






int Function::type() const
{
   return BlockFactory::FunctionType;
}






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






QList<int> Function::buildList() const
{
   QList<int> ret;
   ret << BlockFactory::VariableType;
   if ( !_virtual ) ret << BlockFactory::TemplateType;
   return ret;
}






unique_ptr<QWidget> Function::makeView() const
{
   return unique_ptr<QWidget>(new View::Function(this));
}






unique_ptr<AbstractEdit> Function::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Function(this));
}






QString Function::returnType() const
{
   return variableType();
}






void Function::setReturnType(const QString& type)
{
   setVariableType(type);
}






QString Function::returnDescription() const
{
   return _returnDescription;
}






void Function::setReturnDescription(const QString& description)
{
   if ( _returnDescription != description )
   {
      _returnDescription = description;
      emit modified();
   }
}






bool Function::isVirtual() const
{
   return _virtual;
}






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
      notifyOfNameChange();
      emit modified();
   }
}






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






bool Function::isConst() const
{
   return _const;
}






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
      notifyOfNameChange();
      emit modified();
   }
}






bool Function::isNoExcept() const
{
   return _noExcept;
}






void Function::setNoExcept(bool isNoExcept)
{
   if ( _noExcept != isNoExcept )
   {
      _noExcept = isNoExcept;
      notifyOfNameChange();
      emit modified();
   }
}






bool Function::isOverride() const
{
   return _override;
}






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
      notifyOfNameChange();
      emit modified();
   }
}






bool Function::isFinal() const
{
   return _final;
}






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
      notifyOfNameChange();
      emit modified();
   }
}






bool Function::isAbstract() const
{
   return _abstract;
}






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
      notifyOfNameChange();
      emit modified();
   }
}






bool Function::isMethod() const
{
   return isClassMember();
}






bool Function::hasAnyTemplates() const
{
   if ( hasTemplates() ) return true;
   if ( parent()->type() == BlockFactory::AccessType )
   {
      return parent()->parent()->cast<Class>(BlockFactory::ClassType)->hasAnyTemplates();
   }
   return false;
}






bool Function::hasTemplates() const
{
   return containsType(BlockFactory::TemplateType);
}






QStringList Function::operations() const
{
   return _operations;
}






void Function::setOperations(const QStringList& operations)
{
   if ( _operations != operations )
   {
      _operations = operations;
      emit modified();
   }
}






QList<Variable*> Function::arguments() const
{
   return makeListOfType<Variable>(BlockFactory::VariableType);
}






QList<Template*> Function::templates() const
{
   return makeListOfType<Template>(BlockFactory::TemplateType);
}






void Function::childNameChanged(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Function::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Function::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Function::readData(const QDomElement& data, int version)
{
   Variable::readData(data,version);
   switch (version)
   {
   case 0:
      readVersion0(data);
      break;
   case 1:
      readVersion1(data);
      break;
   default:
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Unknown verison number %1 given for reading block."));
         throw e;
      }
   }
}






int Function::writeVersion() const
{
   return _version;
}






QDomElement Function::writeData(QDomDocument& document) const
{
   QDomElement ret {Variable::writeData(document)};
   if ( _virtual ) ret.appendChild(makeElement(document,_virtualTag,true));
   if ( _const ) ret.appendChild(makeElement(document,_constTag,true));
   if ( _noExcept ) ret.appendChild(makeElement(document,_noExceptTag,true));
   if ( _override ) ret.appendChild(makeElement(document,_overrideTag,true));
   if ( _final ) ret.appendChild(makeElement(document,_finalTag,true));
   if ( _abstract ) ret.appendChild(makeElement(document,_abstractTag,true));
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






unique_ptr<AbstractBlock> Function::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Function);
}






void Function::copyDataFrom(const AbstractBlock* object)
{
   if ( const Function* object_ = qobject_cast<const Function*>(object) )
   {
      Variable::copyDataFrom(object);
      _returnDescription = object_->_returnDescription;
      _virtual = object_->_virtual;
      _const = object_->_const;
      _noExcept = object_->_noExcept;
      _override = object_->_override;
      _final = object_->_final;
      _abstract = object_->_abstract;
      _operations = object_->_operations;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






QString Function::fullName(const QString& returnType, const QString& name) const
{
   QString ret;
   if ( !returnType.isEmpty() ) ret.append(returnType).append(" ");
   ret.append(name).append("(");
   bool first {true};
   const QList<Variable*> variableList {arguments()};
   for (auto variable : variableList)
   {
      if ( first ) first = false;
      else ret.append(",");
      ret.append(variable->variableType());
      if ( variable->hasInitializer() ) ret.append("(=)");
   }
   return ret.append(")").append(attributes());
}






QString Function::attributes() const
{
   QString ret;
   if ( _const ) ret.append("C");
   if ( _noExcept ) ret.append("N");
   if ( _override ) ret.append("O");
   if ( _final ) ret.append("F");
   if ( _abstract ) ret.append("0");
   if ( !ret.isEmpty() ) ret.prepend(" ");
   return ret;
}






void Function::readVersion0(const QDomElement& data)
{
   _operations.clear();
   _returnDescription.clear();
   QList<QDomElement> operations;
   DomElementReader reader(data);
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






void Function::readVersion1(const QDomElement& data)
{
   _virtual = false;
   _const = false;
   _noExcept = false;
   _override = false;
   _final = false;
   _abstract = false;
   _returnDescription.clear();
   _operations.clear();
   QList<QDomElement> operations;
   DomElementReader reader(data);
   reader.set(_virtualTag,&_virtual,false);
   reader.set(_constTag,&_const,false);
   reader.set(_noExceptTag,&_noExcept,false);
   reader.set(_overrideTag,&_override,false);
   reader.set(_finalTag,&_final,false);
   reader.set(_abstractTag,&_abstract,false);
   reader.set(_returnDescriptionTag,&_returnDescription,false);
   reader.set(_operationTag,&operations,false);
   reader.read();
}
