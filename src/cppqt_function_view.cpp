#include "cppqt_function_view.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given function block. 
 *
 * @param block Function block this new view displays. 
 */
Function::View::View(const Function* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's function block. 
 *
 * @return HTML rich text that displays the body of this view's function block. 
 */
QString Function::View::displayText()
{
   // Return as HTML this view's function block's arguments, return, templates, 
   // description, properties, and operations in that order. 
   return displayArguments().append(displayReturn())
                            .append(displayTemplates())
                            .append(displayDescription())
                            .append(displayOperations())
                            .append(displayProperties());
}






/*!
 * Returns a HTML string that displays any arguments this view's function block 
 * contains. If this view's function block has no arguments then this returns an 
 * empty string. 
 *
 * @return HTML string that displays any arguments this view's function block 
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

   // Append a HTML title. 
   ret.append(tr("<h3>Arguments</h3>"));

   // Iterate through all child variable blocks. 
   for (auto variable : list)
   {
      // If the child block has an initial value set its HTML display string. 
      QString initializer;
      if ( variable->hasInitializer() ) initializer = QString(" = ").append(variable->initializer());

      // Append the child variable information as HTML including the possible initial 
      // value. 
      ret.append(tr("<p><u>%1</u> <b>%2</b>%3 : %4</p>")
                 .arg(variable->variableType().replace("<","&lt;"))
                 .arg(variable->Base::name())
                 .arg(initializer)
                 .arg(variable->description()));
   }

   // Return the arguments HTML string. 
   return ret;
}






/*!
 * Returns a HTML string that displays any properties this view's function block 
 * has set. If this view's function block has no properties set then this returns 
 * an empty string. 
 *
 * @return HTML string that displays any properties this view's function block has 
 *         set or an empty string if no properties are set. 
 */
QString Function::View::displayProperties()
{
   // Create an empty string list, appending any properties this view's function 
   // block has set. 
   QStringList list;
   if ( _block->isDefault() ) list << "default";
   if ( _block->isExplicit() ) list << "explicit";
   if ( _block->isVirtual() ) list << "virtual";
   if ( _block->isConst() ) list << "const";
   if ( _block->isNoExcept() ) list << "noexcept";
   if ( _block->isOverride() ) list << "override";
   if ( _block->isFinal() ) list << "final";
   if ( _block->isAbstract() ) list << "abstract(= 0)";

   // If the string list is empty then return an empty string. 
   if ( list.isEmpty() ) return QString();

   // Else this view's block has set properties so return a HTML string displaying 
   // those set properties. 
   else return tr("<h3>Properties</h3><ul><li>%1</li></ul>").arg(list.join("</li><li>"));
}






/*!
 * Returns a HTML string that displays the return type field for this view's 
 * function block. 
 *
 * @return HTML string that displays the return type field of this view's function 
 *         block. 
 */
QString Function::View::displayReturn()
{
   // Get the return type of this view's function block. 
   QString returnType {_block->returnType()};

   // If the return type is empty or is equal to "void" then return an empty string. 
   if ( returnType.isEmpty() || returnType == QString("void") ) return QString();

   // Else this view's function block has a return type so return a HTML string 
   // displaying that return type and its description. 
   else return tr("<h3>Return</h3><p><u>%1</u> : %2</p>")
               .arg(returnType.replace("<","&lt;"))
               .arg(_block->returnDescription());
}






/*!
 * Returns a HTML string that displays the number of operations of this view's 
 * function block. 
 *
 * @return HTML string that displays the number of operations of this view's 
 *         function block. 
 */
QString Function::View::displayOperations()
{
   // Return a HTML string displaying the number of operations. 
   return tr("<i>%n operation(s)</i>",0,_block->operations().size());
}
