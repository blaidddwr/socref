#include "glsl_variable.h"
#include "glsl_variable_view.h"



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
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<BasicBlock::View> Variable::makeBasicView() const
{
   return new View(this);
}
