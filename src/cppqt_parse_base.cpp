#include "cppqt_parse_base.h"
#include <QStack>
#include <QRegularExpression>
#include "cppqt_template.h"
#include "cppqt_factory.h"
#include "cppqt_class.h"
#include "cppqt_namespace.h"
#include "cppqt_template.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Makes and returns a string list of output lines from the given text with the 
 * given justification. The lines are formatted as paragraphs, using double newline 
 * characters to separate the paragraphs. The justification is used to add extra 
 * space to every other line after the first one. This does not returning the 
 * opening and closing comment lines. 
 *
 * @param text The text that is used to make and return the formatted output lines. 
 *
 * @param justified The justification used for adding extra spaces before every 
 *                  output line after the first. 
 *
 * @return Formatted list of output lines made from the given text with the given 
 *         justification. 
 */
QStringList Base::makeComment(const QString& text, int justified)
{
   // Make sure the given justified value is valid. 
   Q_ASSERT(justified >= 0);

   // Get the maximum columns setting and create a new string list that will be 
   // returned. 
   int max {Settings::instance().maxColumns()};
   QStringList ret;

   // Make sure the given text is not empty. 
   if ( text.isEmpty() )
   {
      return ret;
   }

   // Split the given text into paragraphs, using double newline characters as the 
   // separator and skipping empty paragraphs. 
   QStringList paragraphs {text.split("\n\n",QString::SkipEmptyParts)};

   // Iterate through all the paragraphs. 
   bool first {true};
   for (int i = 0; i < paragraphs.size() ;++i)
   {
      // Split the paragraph into its individual words. 
      QStringList words {paragraphs.at(i).split(QRegularExpression("\\s+"))};

      // Keep working until all words have been processed. 
      while ( !words.isEmpty() )
      {
         // Initialize the total number of columns used and the next output line. 
         int total {words.first().size()};
         QString line {" * "};

         // Check if this is the first line. 
         if ( first ) first = false;

         // Else this is not the first line so append the number of justification spaces to 
         // the output line and add the number of columns used to the total. 
         else
         {
            line += QString(justified,QChar(' '));
            total += justified;
         }

         // Append the first word to the output line. The columns used was already added to 
         // the total when it was initialized. 
         line.append(words.takeFirst()).append(" ");

         // Keep working until there are no more words to process or the output line has 
         // reached the maximum number of columns. 
         while ( !words.isEmpty() && (total + words.first().size() + 1) <= max )
         {
            // Add the number of columns the next word will use to the total and then add the 
            // next word to the output line. 
            total += words.first().size() + 1;
            line += words.takeFirst() + QStringLiteral(" ");
         }

         // Add the finished output line to the return string list. 
         ret << line;
      }

      // If this is not the last paragraph add a separator line for the next one. 
      if ( i != (paragraphs.size() - 1) ) ret << QString(" * ");
   }

   // Returned the formatted list of output lines. 
   return ret;
}






/*!
 * Makes and returns a string list of output lines that are the comments of 
 * template arguments for the given block. If the given block has no template 
 * blocks then an empty string list is returned. This does not returning the 
 * opening and closing comment lines. 
 *
 * @param block The block whose template children blocks is used to make and return 
 *              template argument comments. 
 *
 * @return List of template argument comments output lines. 
 */
QStringList Base::makeTemplateComments(const AbstractBlock* block)
{
   // Make sure the given block pointer is not null. 
   Q_CHECK_PTR(block);

   // Create a string list that will be returned. 
   QStringList ret;

   // Iterate through all of the given block's template child blocks. 
   for (auto child : block->makeListOfType<Template>(Factory::TemplateType))
   {
      // Add a blank comment line to the output lines. 
      ret << QStringLiteral(" *");

      // Create a new comment string with the justified part of the string and then get 
      // the number of justified spaces required. 
      QString comment {QStringLiteral("@tparam ") + child->baseName() + QStringLiteral(" ")};
      int justified {comment.size()};

      // Add the final part of the comment string and then make formatted comment output 
      // lines from the comment string with the appropriate justification. 
      comment += child->description();
      ret << makeComment(comment,justified);
   }

   // Return the output line list of template argument comments. 
   return ret;
}






/*!
 * Makes and returns a string that is the scope required for the given block in its 
 * source file definition. This will either return the scope of the first namespace 
 * of the block if it is not part of a class or the full scope of all classes if it 
 * is. 
 *
 * @param block The block whose scope is returned for its source file definition. 
 *
 * @return The scope required for the given block in its source file definition. 
 */
QString Base::makePreScope(const AbstractBlock* block)
{
   // Make sure the given block pointer is not null. 
   Q_CHECK_PTR(block);

   // Get the parent of the given block and make sure it is not null. 
   AbstractBlock* parent {block->parent()};
   Q_CHECK_PTR(parent);

   // If the parent is the root block then return an empty string. 
   if ( !parent->parent() ) return QString();

   // If the parent is a namespace block then return the namespace scope. 
   if ( parent->type() == Factory::NamespaceType )
   {
      return parent->name() + QStringLiteral("::");
   }

   // Else if the parent is an access block then return the class scope. 
   else if ( parent->type() == Factory::AccessType ) return makePreClassScope(parent);

   // Else the parent is an invalid type so throw an exception. 
   else Q_ASSERT(false);
}






/*!
 * Makes and returns a string that is the class scope required for the given block 
 * in its source file definition. If the given block is not part of a class then an 
 * empty string is returned. 
 *
 * @param block The block whose class scope is returned for its source file 
 *              definition. 
 *
 * @return The class scope required for the given block in its source file 
 *         definition or an empty string is the given block is not part of a class. 
 */
QString Base::makePreClassScope(const AbstractBlock* block)
{
   // Make sure the given block pointer is not null. 
   Q_CHECK_PTR(block);

   // Create a new return string and a class pointer stack. 
   QString ret;
   QStack<const Class*> classes;

   // Iterate through the given block's parents. 
   while ( block->parent() )
   {
      // If the parent is a namespace type then stop iterating through parents. 
      if ( block->type() == Factory::NamespaceType ) break;

      // Else if the parent is a class then push its pointer to the stack. 
      else if ( const Class* valid = qobject_cast<const Class*>(block) ) classes.push(valid);

      // Iterate to the next parent. 
      block = block->parent();
   }

   // Keep going until the stack is empty. 
   while ( !classes.isEmpty() )
   {
      // Append the scope of the class to the return string, including any template 
      // arguments. 
      const Class* classBlock {classes.pop()};
      ret += classBlock->baseName() + makeTemplateArguments(classBlock) + QStringLiteral("::");
   }

   // Return the class scope string. 
   return ret;
}






/*!
 * Makes and returns a string that is the template declaration for the given block. 
 * The template declaration is found by finding the first parent of the given block 
 * that contains template blocks and creating the declaration from those blocks. If 
 * no templates are found before the first namespace block is encountered then an 
 * empty string is returned. 
 *
 * @param block The block whose template declaration is returned. 
 *
 * @return Template declaration for the given block or an empty string if no 
 *         templates are found. 
 */
QString Base::makeTemplateDeclaration(const AbstractBlock* block)
{
   // Make sure the given block pointer is not null. 
   Q_CHECK_PTR(block);

   // Iterate through the parents of the given block. 
   while ( block->parent() )
   {
      // If the parent is a namespace block then stop iterating through parents. 
      if ( block->type() == Factory::NamespaceType ) break;

      // Else if the parent block contains template child blocks then return its 
      // declarative template arguments string. 
      else if ( block->containsType(Factory::TemplateType) )
      {
         return makeTemplateArguments(block,true);
      }

      // Iterate to the next parent. 
      block = block->parent();
   }

   // No template arguments were found so return an empty string. 
   return QString();
}






/*!
 * Makes and returns a string that is the template arguments of the given block. 
 * The string can either be in normal form or declarative form. The normal form is 
 * meant to be appended to block's name while the declarative is meant to be added 
 * before the name. If the given block has no templates then an empty string is 
 * returned. 
 *
 * @param block The block whose child template blocks are used to make the returned 
 *              template arguments string. 
 *
 * @param declarative True to make the returned string in declarative form or false 
 *                    to make it in normal form. 
 *
 * @return Template arguments of the given block in normal or declarative form. An 
 *         empty string if the given block has no templates. 
 */
QString Base::makeTemplateArguments(const AbstractBlock* block, bool declarative)
{
   // Make sure the given block pointer is not null. 
   Q_CHECK_PTR(block);

   // Create a new return string. 
   QString ret;

   // Get the given block's list of template block children and make sure it is not 
   // empty. 
   const QList<Template*> list {block->makeListOfType<Template>(Factory::TemplateType)};
   if ( !list.isEmpty() )
   {
      // Append the opening carrot, including the template declaration if this is 
      // declarative. 
      if ( declarative ) ret += QStringLiteral("template");
      ret += QChar('<');

      // Iterate through the list of template block children. 
      bool first {true};
      for (auto item : list)
      {
         // Check to see if this is the first argument. 
         if ( first ) first = false;

         // Else this is not the first argument so append a separating comma. 
         else ret += QChar(',');

         // Append the template child block's name, including its type if this is 
         // declarative. 
         if ( declarative ) ret +=  item->templateType() + QChar(' ');
         ret += item->baseName();
      }

      // Append the closing carrot. 
      ret += QChar('>');
   }

   // Return the template arguments string. 
   return ret;
}






/*!
 * Constructs a new base parser with the given parser as its parent. The base 
 * parser is designed so it cannot be a root parser and must have a parent. 
 *
 * @param parent The parent parser for this new base parser. 
 */
Base::Base(AbstractParser* parent):
   AbstractParser(parent)
{}






/*!
 * Implements _AbstractParser_ interface. This implementation does nothing and 
 * overrides its interface with the final keyword because base classes should use 
 * their more specialized output interfaces. 
 */
void Base::makeOutput()
{}
