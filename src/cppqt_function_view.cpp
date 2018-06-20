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
   return displayArguments().append(displayProperties())
                            .append(displayTemplates())
                            .append(displayDescription())
                            .append(displayReturn())
                            .append(displayOperations());
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
   // Create an empty string _ret_ and then a pointer list _list_ of this object's 
   // function block's arguments. If _list_ is empty then return _ret_. 
   QString ret;
   const QList<CppQt::Variable*> list {_block->arguments()};
   if ( list.isEmpty() ) return ret;

   // Append an HTML title to _ret_, then all arguments from _list_ as an HTML list 
   // to _ret_. Include the variable type, base name, initialized value, and 
   // description for each argument listing. 
   ret.append("<h3>Arguments</h3>");
   for (auto variable : list)
   {
      ret.append("<p><b>")
         .append(variable->variableType().replace("<","&lt;"))
         .append(" ")
         .append(variable->Base::name())
         .append("</b>");
      if ( variable->hasInitializer() ) ret.append(" = ").append(variable->initializer());
      ret.append(" : ").append(variable->description()).append("</p>");
   }

   // Return _ret_. 
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
   // Create an empty string _ret_ and then an empty string list _list_. Append any 
   // properties this object's function block has set to _list_. If _list_ is empty 
   // then return _ret_. 
   QString ret;
   QStringList list;
   if ( _block->isDefault() ) list << "default";
   if ( _block->isExplicit() ) list << "explicit";
   if ( _block->isVirtual() ) list << "virtual";
   if ( _block->isConst() ) list << "const";
   if ( _block->isNoExcept() ) list << "noexcept";
   if ( _block->isOverride() ) list << "override";
   if ( _block->isFinal() ) list << "final";
   if ( _block->isAbstract() ) list << "abstract(= 0)";
   if ( list.isEmpty() ) return ret;

   // Append an HTML title, then all set properties as an HTML list to _ret_, and 
   // then return _ret_. 
   ret.append("<h3>")
      .append(tr("Properties"))
      .append("</h3><ul><li>")
      .append(list.join("</li><li>"))
      .append("</li></ul>");
   return ret;
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
   // Create and return a string that contains HTML for this object's function return 
   // type field along with a title. Replace any special carrot characters so it 
   // doesn't break the HTML code. 
   QString ret;
   QString returnType {_block->returnType()};
   if ( returnType.isEmpty() || returnType == QString("void") ) return ret;
   ret.append("<h3>Return</h3><p><b>")
      .append(returnType.replace("<","&lt;"))
      .append(" :</b> ")
      .append(_block->returnDescription())
      .append("</p>");
   return ret;
}






/*!
 * Returns a HTML string that displays the operations of this view's function 
 * block. If the function block has operations then an empty string is returned. 
 *
 * @return HTML string that displays the operations of this view's function block 
 *         or an empty string if there are no operations. 
 */
QString Function::View::displayOperations()
{
   // Create an empty string _ret_ and then get this view's function block's 
   // operations saving it to _list_. If _list_ is empty then return _ret_. 
   QString ret;
   const QStringList list {_block->operations()};
   if ( list.isEmpty() ) return ret;

   // Append an HTML title and then the list of operations stored in _list_ as an 
   // HTML list to _ret_. 
   ret.append("<h3>Operations</h3>");
   for (int i = 0; i < list.size() ;++i)
   {
      ret.append("<p><b>#")
         .append(QString::number(i + 1))
         .append("</b> ")
         .append(parseBoldMarkers(list.at(i)))
         .append("</p>");
   }

   // Return _ret_. 
   return ret;
}
