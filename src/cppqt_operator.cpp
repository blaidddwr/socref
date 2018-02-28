#include "cppqt_operator.h"
#include <exception.h>
#include "cppqt_view_operator.h"
#include "cppqt_edit_operator.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Operator::_operationTag {"operation"};






Operator::Operator(const QString& returnType)
{
   setReturnType(returnType);
}






QString Operator::name() const
{
   return fullName(returnType(),QString("operator").append(_operation));
}






int Operator::type() const
{
   return BlockFactory::OperatorType;
}






QIcon Operator::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/operator.svg");
      virtual_ = QIcon(":/icons/voperator.svg");
      abstract = QIcon(":/icons/aoperator.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






QList<int> Operator::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






unique_ptr<QWidget> Operator::makeView() const
{
   return unique_ptr<QWidget>(new View::Operator(this));
}






unique_ptr<AbstractEdit> Operator::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Operator(this));
}






QString Operator::operation() const
{
   return _operation;
}






void Operator::setOperation(const QString& operation)
{
    if ( _operation != operation )
    {
       _operation = operation;
       notifyOfNameChange();
       emit modified();
    }
}






void Operator::readData(const QDomElement& data)
{
   Function::readData(data);
   DomElementReader reader(data);
   _operation = reader.attribute(_operationTag);
}






QDomElement Operator::writeData(QDomDocument& document) const
{
   QDomElement ret {Function::writeData(document)};
   ret.setAttribute(_operationTag,_operation);
   return ret;
}






unique_ptr<AbstractBlock> Operator::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Operator);
}






void Operator::copyDataFrom(const AbstractBlock* object)
{
   if ( const Operator* object_ = qobject_cast<const Operator*>(object) )
   {
      Function::copyDataFrom(object);
      _operation = object_->_operation;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
