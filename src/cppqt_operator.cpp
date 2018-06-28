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
   return fullName(!isVoidReturn(),QString("operator").append(Base::name()));
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Operator::icon() const
{
   // 
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;

   // 
   if ( !isLoaded )
   {

      // 
      regular = QIcon(":/icons/operator.svg");
      virtual_ = QIcon(":/icons/voperator.svg");
      abstract = QIcon(":/icons/aoperator.svg");
      isLoaded = true;
   }

   // 
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
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Operator::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
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
   return Base::name();
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
 * This interface checks to make sure the given name is a valid name for this block 
 * type, returning true if it is valid. This implementation makes sure it is a 
 * basic C++ alphanumeric name. 
 *
 * @param value The name value whose syntax is checked to be valid or not. 
 *
 * @return True if the given name is valid or false otherwise. 
 */
bool Operator::checkName(const QString& value)
{
   Q_UNUSED(value)
   return true;
}
