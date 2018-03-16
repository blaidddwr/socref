#include "cppqt_operator.h"
#include <exception.h>
#include "cppqt_view_operator.h"
#include "cppqt_edit_operator.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Operator::_operationTag {"operation"};
const char* Operator::_operatorTag {"operator"};






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






void Operator::readData(const QDomElement& data, int version)
{
   Function::readData(data,version);
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






int Operator::writeVersion() const
{
   return _version;
}






QDomElement Operator::writeData(QDomDocument& document) const
{
   QDomElement ret {Function::writeData(document)};
   ret.appendChild(makeElement(document,_operatorTag,_operation));
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






void Operator::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _operation = reader.attribute(_operationTag);
}






void Operator::readVersion1(const QDomElement& data)
{
   DomElementReader reader(data);
   reader.set(_operatorTag,&_operation);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
}
