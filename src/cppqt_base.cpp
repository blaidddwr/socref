#include "cppqt_base.h"
#include <QJsonObject>
#include <socutil/sut_exceptions.h>
#include "cppqt_factory.h"
#include "cppqt_typelist.h"
#include "cppqt_type.h"



using namespace Sut;
using namespace CppQt;
//






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
QJsonObject Base::typeList() const
{
   // Create a new JSON return object and get the parent of this block. 
   QJsonObject ret;
   AbstractBlock* block {parent()};

   // Iterate up the chain of block parents until it is a null pointer. 
   while ( block )
   {
      // Iterate through all type list children blocks of this block and add their type 
      // lists to the return JSON object. 
      const QList<TypeList*> list {block->makeListOfType<TypeList>(Factory::TypeListType)};
      for (auto typeList: list)
      {
         ret.insert(typeList->name(),addList(*typeList));
      }

      // Move up to the next block parent in the chain. 
      block = block->parent();
   }

   // Return the fully populated JSON object of types. 
   return ret;
}






/*!
 * Adds the given list of types to a new JSON object that is returned. This also 
 * recursively adds any child type lists within the given type list block to the 
 * returned JSON object as an object within it. 
 *
 * @param block The type list block that is used to populate the given JSON object 
 *              with all types and child type lists it contains. 
 *
 * @return A new JSON object that contains a full list of all types contained in 
 *         the given type list block, including all children type lists. 
 */
QJsonObject Base::addList(const TypeList& block)
{
   // Create a new JSON object that will be populated and returned. 
   QJsonObject ret;

   // Iterate through all children of the given type list block. 
   for (auto child: block.list())
   {
      // If the child block is a type list then add a new JSON object to the given 
      // object created and populated by recursively calling this method. 
      if ( TypeList* valid = child->cast<TypeList>(Factory::TypeListType) )
      {
         ret.insert(valid->name(),addList(*valid));
      }

      // Else if the block is a simple type then add it. 
      else if ( Type* valid = child->cast<Type>(Factory::TypeType) )
      {
         ret.insert(valid->name(),QJsonValue());
      }
   }

   // Return the populated JSON object. 
   return ret;
}
