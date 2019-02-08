#include "glsl_shaderview.h"
#include "glsl_shader.h"



using namespace GLSL;
//






/*!
 * Constructs a new view with the given shader block. 
 *
 * @param block The shader block that this new view displays. 
 */
ShaderView::ShaderView(const Shader* block):
   Basic::BlockView(block)
{}






/*!
 * Returns rich text that displays the number of operations for this view's 
 * function block. 
 *
 * @return Rich text that displays the number of operations for this view's 
 *         function block. 
 */
QString ShaderView::displayOperations()
{
   return tr("<i>%n operation(s)</i>",nullptr,block<Shader>().operations().size());
}
