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
//



/*!
 */
const QStringList Operator::_fields {"operator"};






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Operator::type() const
{
   return BlockFactory::OperatorType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Operator::name() const
{
   return fullName(!isVoidReturn(),QString("operator").append(_operator));
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
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






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Operator::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Operator::makeView() const
{
   return unique_ptr<QWidget>(new View::Operator(this));
}






/*!
 */
int Operator::fieldSize() const
{
   return Field::Total;
}






/*!
 *
 * @param index  
 */
AbstractBlock::Field Operator::fieldType(int index) const
{
   switch (index)
   {
   case Field::OperatorType: return AbstractBlock::Field::String;
   default: return Function::fieldType(index);
   }
}






/*!
 *
 * @param index  
 */
QVariant Operator::field(int index) const
{
   switch (index)
   {
   case Field::OperatorType: return _operator;
   default: return Function::fieldType(index);
   }
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Operator::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Operator(this));
}






/*!
 *
 * @param returnType  
 */
Operator::Operator(const QString& returnType):
   Function(returnType,QString())
{}






/*!
 */
QString Operator::operation() const
{
   return _operator;
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Operator::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Operator);
}






/*!
 *
 * @param index  
 */
void Operator::fieldModified(int index)
{
   switch (index)
   {
   case Field::OperatorType:
      notifyModified();
      notifyNameModified();
      break;
   default:
      Function::fieldModified(index);
      break;
   }
}






/*!
 *
 * @param index  
 *
 * @param value  
 */
void Operator::quietlySetField(int index, const QVariant& value)
{
   switch (index)
   {
   case Field::OperatorType:
      _operator = value.toString();
      break;
   default:
      Function::quietlySetField(index,value);
      break;
   }
}






/*!
 */
QStringList Operator::fields() const
{
   static QStringList ret;
   if ( ret.isEmpty() )
   {
      ret.append(Function::fields());
      ret.append(_fields);
   }
   return ret;
}
