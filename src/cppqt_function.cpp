#include "cppqt_function.h"
#include "cppqt_view_function.h"
#include "cppqt_edit_function.h"
#include "cppqt_variable.h"
#include "exception.h"
#include "cppqt_gui_typedialog.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "cppqt_template.h"
#include "cppqt_common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






Function::Function(const QString& name):
   Variable(name)
{}






Function::Function(const QString& returnType, const QString& name):
   Variable(returnType,name)
{}






QString Function::name() const
{
   QString ret {templateName(this)};
   if ( _virtual ) ret.append("virtual ");
   ret.append(properties());
   ret.append(returnType()).append(" ").append(Base::name()).append("(");
   bool first {true};
   const QList<Variable*> variableList {arguments()};
   for (auto variable : variableList)
   {
      if ( first ) first = false;
      else ret.append(",");
      ret.append(variable->variableType());
      if ( variable->hasInitializer() ) ret.append(" = ").append(variable->initializer());
   }
   ret.append(")");
   if ( _const ) ret.append(" const");
   if ( _noExcept ) ret.append(" noexcept");
   if ( _override ) ret.append(" override");
   if ( _final ) ret.append(" final");
   if ( _abstract ) ret.append(" = 0");
   return ret;
}






unique_ptr<AbstractBlock> Function::makeCopy() const
{
   unique_ptr<Function> ret {new Function};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_returnDescription = _returnDescription;
   ret->_virtual = _virtual;
   ret->_const = _const;
   ret->_noExcept = _noExcept;
   ret->_override = _override;
   ret->_final = _final;
   ret->_abstract = _abstract;
   ret->_operations = _operations;
   return ret;
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






bool Function::hasTemplates() const
{
   return hasChildOfType(BlockFactory::TemplateType);
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
   return makeChildListOfType<Variable>(BlockFactory::VariableType);
}






QList<Template*> Function::templates() const
{
   return makeChildListOfType<Template>(BlockFactory::TemplateType);
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






void Function::readData(const QDomElement& data)
{
   _operations.clear();
   _returnDescription.clear();
   Variable::readData(data);
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






QDomElement Function::writeData(QDomDocument& document) const
{
   QDomElement ret {Variable::writeData(document)};
   if ( _virtual ) ret.setAttribute(_virtualTag,_virtual);
   if ( _const ) ret.setAttribute(_constTag,_const);
   if ( _noExcept ) ret.setAttribute(_noExceptTag,_noExcept);
   if ( _override ) ret.setAttribute(_overrideTag,_override);
   if ( _final ) ret.setAttribute(_finalTag,_final);
   if ( _abstract ) ret.setAttribute(_abstractTag,_abstract);
   if ( !_returnDescription.isEmpty() )
   {
      QDomElement element {document.createElement(_returnDescriptionTag)};
      element.appendChild(document.createTextNode(_returnDescription));
      ret.appendChild(element);
   }
   for (auto operation : qAsConst(_operations))
   {
      QDomElement element {document.createElement(_operationTag)};
      element.appendChild(document.createTextNode(operation));
      ret.appendChild(element);
   }
   return ret;
}
