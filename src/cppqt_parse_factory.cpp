#include "cppqt_parse_factory.h"
#include <exception.h>
#include "cppqt_parse_global.h"
#include "cppqt_parse_header.h"
#include "cppqt_parse_source.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "abstractblock.h"



using namespace std;
using namespace CppQt;
using namespace CppQt::Parse;
//






/*!
 * Implements _AbstractParserFactory_ interface. 
 *
 * @param name See interface docs. 
 *
 * @param extension See interface docs. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractParser> Factory::make(const QString& name, const QString& extension) const
{
   // Make sure the given file name is not empty. 
   if ( name.isEmpty() ) return nullptr;

   // Create the is header flag and set its value. 
   bool isHeader {extension == QStringLiteral("h")};

   // Create the list of namespace names from the given file name. 
   QStringList names {name.split('_')};

   // Check to see if this is a common file. 
   if ( names.back() == QString("common") )
   {
      // Remove the common name from the list and return the found common parser. 
      names.takeLast();
      return unique_ptr<AbstractParser>(findCommon(_root,names,name,isHeader));
   }

   // Else this is a global or class file so return the found header/source parser. 
   else return unique_ptr<AbstractParser>(find(_root,names,name,isHeader));
}






/*!
 * Constructs a new C++/Qt parser factory with the given root block. 
 *
 * @param root The root block used by this parser factory. 
 */
Factory::Factory(const AbstractBlock* root):
   _root(qobject_cast<const Namespace*>(root))
{
   // Make sure the given root pointer is not null. 
   if ( !_root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Root block is type '%1' intead of namespace.")
               .arg(root->factory().name(root->type())));
      throw e;
   }
}






/*!
 * Attempts to find the namespace or class that matches the given file information. 
 * The appropriate parser is returned or a null pointer if no matching block is 
 * found. This recursively calls itself to resolve any scoped namespace for 
 * matching. 
 *
 * @param current The current namespace being inspected. 
 *
 * @param names The name fragments of the given file used for namespace resolution. 
 *
 * @param name The full name of the file being parsed. 
 *
 * @param isHeader True if the file is a header or false if it is a source file. 
 *
 * @param index The current index matched so far in the given names variable for 
 *              namespace resolution. 
 *
 * @return Pointer to the appropriate parser that matched the given file 
 *         information or null if no match is found. 
 */
AbstractParser* Factory::find(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, int index) const
{
   // Make sure the given namespace pointer and index are valid. 
   if ( index < 0 || index > names.size() || !current ) return nullptr;

   // Check to see if we have reached the end of the names list. 
   if ( index == names.size() )
   {
      // If the current block is a namespace then return a global parser. 
      if ( current->type() == BlockFactory::NamespaceType )
      {
         return new Parse::Global(current);
      }

      // Else the current block is a class so return its header or source parser. 
      else
      {
         if ( isHeader ) return new Parse::Header(current,name);
         else return new Parse::Source(current,name);
      }
   }

   // Else attempt to find the next child namespace and if found recursively call 
   // this method on it. 
   else if ( const Namespace* valid = findNamespace(current,names.at(index++)) )
   {
      return find(valid,names,name,isHeader,index);
   }

   // Else namespace resolution failed so return null. 
   else return nullptr;
}






/*!
 * Attempts to find the namespace that matches the given file information. The 
 * appropriate common parser is returned or a null pointer if no matching block is 
 * found. This recursively calls itself to resolve any scoped namespace for 
 * matching. 
 *
 * @param current The current namespace being inspected. 
 *
 * @param names The name fragments of the given file used for namespace resolution. 
 *
 * @param name The full name of the file being parsed. 
 *
 * @param isHeader True if the file is a header or false if it is a source file. 
 *
 * @param index The current index matched so far in the given names variable for 
 *              namespace resolution. 
 *
 * @return Pointer to the common parser that matched the given file information or 
 *         null if no match is found. 
 */
AbstractParser* Factory::findCommon(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, int index) const
{
   // Make sure the given namespace pointer and index are valid. 
   if ( index < 0 || index > names.size() || !current ) return nullptr;

   // Attempt to find the next child namespace. 
   if ( const Namespace* valid = findNamespace(current,names.at(index)) )
   {
      // Check if this is the end of the list of names. 
      if ( ++index == names.size() )
      {
         // If this last block is a namespace then return its common header or source 
         // parser. 
         if ( valid->type() == BlockFactory::NamespaceType )
         {
            if ( isHeader ) return new Parse::Header(valid,name);
            else return new Parse::Source(valid,name);
         }
      }

      // Else name resolution is not complete so recursively call this method. 
      else return findCommon(valid,names,name,isHeader,index);
   }

   // Namespace resolution failed if this is reached so return null. 
   return nullptr;
}






/*!
 * Attempts to find a child namespace contained in the given namespace that matches 
 * the given name, returning a pointer if one if found. 
 *
 * @param current The current namespace being inspected. 
 *
 * @param name The name that is matched with child namespace blocks. 
 *
 * @return Pointer to matching namespace or null if no match is found. 
 */
const Namespace* Factory::findNamespace(const Namespace* current, const QString& name) const
{
   // Iterate through the list of the given namespace block's real children. 
   QList<AbstractBlock*> list {current->realChildren()};
   for (auto child : list)
   {
      // Check if the current child block is a namespace or class. 
      if ( Namespace* valid = qobject_cast<Namespace*>(child) )
      {
         // If the namespace or class name matches the given name then return its pointer. 
         if ( valid->Base::name().toLower() == name ) return valid;
      }
   }

   // No match was found so return null. 
   return nullptr;
}
