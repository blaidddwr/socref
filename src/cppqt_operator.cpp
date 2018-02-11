#include "cppqt_operator.h"
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






unique_ptr<AbstractBlock> Operator::makeCopy() const
{
   unique_ptr<Operator> ret {new Operator};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_operation = _operation;
   return ret;
}






int Operator::type() const
{
   return BlockFactory::OperatorType;
}






QIcon Operator::icon() const
{
   return Function::icon();
}






QList<int> Operator::buildList() const
{
   static QList<int> ret;
   if ( ret.isEmpty() )
   {
      ret << BlockFactory::VariableType;
   }
   return ret;
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
