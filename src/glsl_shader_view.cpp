#include "glsl_shader_view.h"



using namespace GLSL;
//






/*!
 * Constructs a new view with the given shader block. 
 *
 * @param block The shader block that this new view displays. 
 */
Shader::View::View(const Shader* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays the number of operations for this view's 
 * function block. 
 *
 * @return Rich text that displays the number of operations for this view's 
 *         function block. 
 */
QString Shader::View::displayOperations()
{
   return tr("<i>%n operation(s)</i>",nullptr,_block->operations().size());
}
