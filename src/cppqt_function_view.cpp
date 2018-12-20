#include "cppqt_function_view.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given function block. 
 *
 * @param block Function block this new view displays. 
 */
Function::View::View(const Function* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays any arguments this view's function block 
 * contains. If this view's function block has no arguments then this returns an 
 * empty string. 
 *
 * @return Rich text that displays any arguments this view's function block 
 *         contains or an empty string if there are no arguments. 
 */
QString Function::View::displayArguments()
{
   // Create an empty return string. 
   QString ret;

   // Make a pointer list of all children variables of this view's function block and 
   // make sure it is not empty. 
   const QList<CppQt::Variable*> list {_block->arguments()};
   if ( list.isEmpty() ) return ret;

   // Append a title. 
   ret.append(tr("<h3>Arguments</h3>"));

   // Iterate through all child variable blocks. 
   for (auto variable : list)
   {
      // If the child block has an initial value then add its rich text. 
      QString initializer;
      if ( variable->hasInitializer() ) initializer = QString(" = ").append(variable->initializer());

      // Append the child variable information as rich text including the possible 
      // initial value. 
      ret.append(tr("<p><u>%1</u> <b>%2</b>%3 : %4</p>")
                 .arg(variable->variableType().replace("<","&lt;"))
                 .arg(variable->baseName())
                 .arg(initializer)
                 .arg(variable->description()));
   }

   // Return the arguments rich text. 
   return ret;
}






/*!
 * Wrapper method that calls the static method in _CppQt::Template_ of the same 
 * name. 
 *
 * @return See wrapped method docs. 
 */
QString Function::View::displayTemplates()
{
   return Template::displayTemplates(*_block);
}






/*!
 * Returns rich text that displays any properties this view's function block has 
 * set. If this view's function block has no properties set then this returns an 
 * empty string. 
 *
 * @return Rich text that displays any properties this view's function block has 
 *         set or an empty string if no properties are set. 
 */
QString Function::View::displayProperties()
{
   // Create an empty string list, appending any properties this view's function 
   // block has set. 
   QStringList list;
   if ( _block->isDefault() ) list << "Default";
   if ( _block->isDeleted() ) list << "Deleted";
   if ( _block->isQtInvokable() ) list << "Qt Invokable";
   if ( _block->isExplicit() ) list << "Explicit";
   if ( _block->isVirtual() ) list << "Virtual";
   if ( _block->isConst() ) list << "Constant";
   if ( _block->isConstExpr() ) list << "Constant Expression";
   if ( _block->isStatic() ) list << "Static";
   if ( _block->isNoExcept() ) list << "No Exceptions";
   if ( _block->isOverride() ) list << "Override";
   if ( _block->isFinal() ) list << "Final";
   if ( _block->isAbstract() ) list << "Abstract(=0)";

   // If the string list is empty then return an empty string. 
   if ( list.isEmpty() ) return QString();

   // Else this view's block has set properties so return rich text displaying those 
   // set properties. 
   else return tr("<h3>Properties</h3><ul><li>%1</li></ul>").arg(list.join("</li><li>"));
}






/*!
 * Returns rich text that displays the return type field for this view's function 
 * block. 
 *
 * @return Rich text that displays the return type field of this view's function 
 *         block. 
 */
QString Function::View::displayReturn()
{
   // Get the return type of this view's function block. 
   QString returnType {_block->returnType()};

   // If the return type is empty or is equal to "void" then return an empty string. 
   if ( returnType.isEmpty() || returnType == QString("void") ) return QString();

   // Else this view's function block has a return type so return rich text 
   // displaying that return type and its description. 
   else return tr("<h3>Return</h3><p><u>%1</u> : %2</p>")
               .arg(returnType.replace("<","&lt;"))
               .arg(_block->returnDescription());
}






/*!
 * Returns rich text that displays the number of operations of this view's function 
 * block. 
 *
 * @return Rich text that displays the number of operations of this view's function 
 *         block. 
 */
QString Function::View::displayOperations()
{
   // Return rich text displaying the number of operations. 
   return tr("<i>%n operation(s)</i>",0,_block->operations().size());
}
