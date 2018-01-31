#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "exception.h"
#include "cppqt_gui_typedialog.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace CppQt;



const char* Function::_typeTag {"type"};
const char* Function::_descriptionTag {"description"};
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
   QString ret {returnType().append(" ").append(Base::name()).append("(")};
   bool first {true};
   const auto list {arguments()};
   for (auto variable : list)
   {
      if ( first )
      {
         first = false;
      }
      else
      {
         ret.append(", ");
      }
      ret.append(variable->variableType());
   }
   ret.append(")");
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
   _returnType = type;
}






QString Function::returnDescription() const
{
   return _returnDescription;
}






void Function::setReturnDescription(const QString& description)
{
   _returnDescription = description;
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
      if ( Variable* variable = qobject_cast<Variable*>(child) )
      {
         ret.append(variable);
      }
   }
   return ret;
}






void Function::readData(const QDomElement& data)
{
   _operations.clear();
   _returnDescription.clear();
   Base::readData(data);
   QList<QDomElement> operations;
   DomElementReader reader(data);
   _returnType = reader.attribute(_typeTag);
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
   QDomElement ret {document.createElement("na")};
   ret.setAttribute(_typeTag,_returnType);
   if ( !_returnDescription.isEmpty() )
   {
      QDomElement element {document.createElement(_descriptionTag)};
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
