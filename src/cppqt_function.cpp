#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "exception.h"
#include "cppqt_gui_typedialog.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "cppqt_template.h"



using namespace std;
using namespace CppQt;



const char* Function::_preFlagRegExp {"((virtual)|(static))?"};
const char* Function::_postFlagRegExp {"(const)?(\\s+noexcept)?((\\s+override)?(\\s+final)?|(\\s+=\\s0;)?)"};
const char* Function::_typeTag {"type"};
const char* Function::_descriptionTag {"description"};
const char* Function::_preFlagTag {"pre"};
const char* Function::_postFlagTag {"post"};
const char* Function::_operationTag {"operation"};






Function::Function(const QString& name):
   Base(name)
{}






Function::Function(const QString& returnType, const QString& name):
   Base(name),
   _returnType(returnType)
{}






QString Function::name() const
{
   QString ret;
   const auto templateList {templates()};
   if ( !templateList.isEmpty() )
   {
      ret.append("template<");
      bool first {true};
      for (auto template_ : templateList)
      {
         if ( first )
         {
            first = false;
         }
         else
         {
            ret.append(", ");
         }
         ret.append(template_->name());
      }
      ret.append("> ");
   }
   if ( !_preFlags.isEmpty() )
   {
      ret.append(_preFlags).append(" ");
   }
   ret.append(returnType()).append(" ").append(Base::name()).append("(");
   bool first {true};
   const auto variableList {arguments()};
   for (auto variable : variableList)
   {
      if ( first )
      {
         first = false;
      }
      else
      {
         ret.append(",");
      }
      ret.append(variable->variableType());
   }
   ret.append(")");
   if ( !_postFlags.isEmpty() )
   {
      ret.append(" ").append(_postFlags);
   }
   return ret;
}






unique_ptr<AbstractBlock> Function::makeCopy() const
{
   unique_ptr<Function> ret {new Function};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_returnType = _returnType;
   ret->_operations = _operations;
   return ret;
}






int Function::type() const
{
   return BlockFactory::FunctionType;
}






QString Function::elementName() const
{
   return QString("function");
}






QIcon Function::icon() const
{
   static QIcon ret;
   if ( ret.isNull() )
   {
      ret = QIcon(":/icons/function.svg");
   }
   return ret;
}






QList<int> Function::buildList() const
{
   static QList<int> ret;
   if ( ret.isEmpty() )
   {
      ret << BlockFactory::TemplateType;
      ret << BlockFactory::VariableType;
   }
   return ret;
}






QString Function::returnType() const
{
   return _returnType;
}






void Function::setReturnType(const QString& type)
{
   if ( !QRegExp(Gui::TypeDialog::_typeRegExp).exactMatch(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid return type '%1'.").arg(type));
      throw e;
   }
   if ( _returnType != type )
   {
      _returnType = type;
      notifyOfNameChange();
      emit modified();
   }
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
      notifyOfNameChange();
      emit modified();
   }
}






QString Function::preFlags() const
{
   return _preFlags;
}






void Function::setPreFlags(const QString& flags)
{
   if ( !QRegExp(_preFlagRegExp).exactMatch(flags) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function pre flags to '%1'.").arg(flags));
      throw e;
   }
   if ( _preFlags != flags )
   {
      _preFlags = flags;
      notifyOfNameChange();
      emit modified();
   }
}






QString Function::postFlags() const
{
   return _postFlags;
}






void Function::setPostFlags(const QString& flags)
{
   if ( !QRegExp(_postFlagRegExp).exactMatch(flags) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set function post flags to '%1'.").arg(flags));
      throw e;
   }
   if ( _postFlags != flags )
   {
      _postFlags = flags;
      notifyOfNameChange();
      emit modified();
   }
}






QStringList Function::operations() const
{
   return _operations;
}






void Function::setOperations(const QStringList& operations)
{
   _operations = operations;
}






QList<Variable*> Function::arguments() const
{
   QList<Variable*> ret;
   const auto list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::VariableType )
      {
         if ( Variable* variable = qobject_cast<Variable*>(child) )
         {
            ret.append(variable);
         }
      }
   }
   return ret;
}






QList<Template*> Function::templates() const
{
   QList<Template*> ret;
   const auto list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::TemplateType )
      {
         if ( Template* variable = qobject_cast<Template*>(child) )
         {
            ret.append(variable);
         }
      }
   }
   return ret;
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
   Base::readData(data);
   QList<QDomElement> operations;
   DomElementReader reader(data);
   _returnType = reader.attribute(_typeTag);
   _preFlags = reader.attribute(_preFlagTag,false);
   _postFlags = reader.attribute(_postFlagTag,false);
   reader.set(_operationTag,&operations,false);
   reader.set(_descriptionTag,&_returnDescription,false);
   reader.read();
   for (auto operation : qAsConst(operations))
   {
      _operations.append(operation.text());
   }
}






QDomElement Function::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   ret.setAttribute(_typeTag,_returnType);
   if ( !_returnDescription.isEmpty() )
   {
      QDomElement element {document.createElement(_descriptionTag)};
      element.appendChild(document.createTextNode(_returnDescription));
      ret.appendChild(element);
   }
   if ( !_preFlags.isEmpty() )
   {
      ret.setAttribute(_preFlagTag,_preFlags);
   }
   if ( !_postFlags.isEmpty() )
   {
      ret.setAttribute(_postFlagTag,_postFlags);
   }
   for (auto operation : qAsConst(_operations))
   {
      QDomElement element {document.createElement(_operationTag)};
      element.appendChild(document.createTextNode(operation));
      ret.appendChild(element);
   }
   return ret;
}
