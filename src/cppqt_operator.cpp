#include "cppqt_operator.h"
#include <exception.h>
#include "cppqt_operator_edit.h"
#include "cppqt_blockfactory.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Operator::type() const
{
   return BlockFactory::OperatorType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Operator::name() const
{
   // Return the display name of this operator block using the special operator 
   // overloading syntax for its function name. 
   return fullName(!isVoidReturn(),QString("operator").append(Base::name()));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Operator::icon() const
{
   // Initialize the static icons for this block type. 
   static QIcon regularIcon(":/icons/operator.svg");
   static QIcon virtualIcon(":/icons/voperator.svg");
   static QIcon abstractIcon(":/icons/aoperator.svg");

   // Return the appropriate icon for this operator block given its current 
   // properties. 
   if ( isAbstract() ) return abstractIcon;
   else if ( isVirtual() ) return virtualIcon;
   else return regularIcon;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Operator::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Operator::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Returns the operation that this operator block is overloading. 
 *
 * @return Operation that this operator block is overloading. 
 */
QString Operator::operation() const
{
   // Return this block's base name that the operator type uses to store the 
   // operation it overloads. 
   return Base::name();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Operator::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Operator);
}






/*!
 * Implements _CppQt::Base_ interface. This implementation validates any string 
 * because the name for operator blocks is the operator it is overloading. 
 *
 * @param value See interface docs. 
 *
 * @return See interface docs. 
 */
bool Operator::checkName(const QString& value)
{
   // This validates any string no matter what so ignore the given value and return 
   // true. 
   Q_UNUSED(value)
   return true;
}
