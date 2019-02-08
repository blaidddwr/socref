#include "glsl_variable.h"
#include "glsl_variableview.h"



using namespace GLSL;
//






/*!
 * Returns this variable block's type string. 
 *
 * @return This variable block's type string. 
 */
QString Variable::typeString() const
{
   return getString("type");
}






/*!
 * Returns this variable block's list of layout qualifiers. 
 *
 * @return This variable block's list of layout qualifiers. 
 */
QStringList Variable::layout() const
{
   return getString("layout").split('\n',QString::SkipEmptyParts);
}






/*!
 * Implements _Basic::Block_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<Basic::BlockView> Variable::makeBasicView() const
{
   return new VariableView(this);
}
