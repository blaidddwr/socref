#include "cppqt_namespace.h"
#include <memory>
#include <QDomDocument>
#include <exception.h>
#include "cppqt_namespace_view.h"
#include "cppqt_namespace_edit.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the static qt icon _ret_ is null then load this block's icon. 
 *
 * 2. Return _ret_. 
 */
QIcon Namespace::icon() const
{
   // 1
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/namespace.svg");

   // 2
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block type can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Return the build list of all other block types this namespace block type can 
 *    contain. 
 */
QList<int> Namespace::buildList() const
{
   // 1
   return QList<int>
   {
      BlockFactory::NamespaceType
      ,BlockFactory::VariableType
      ,BlockFactory::FunctionType
      ,BlockFactory::ClassType
      ,BlockFactory::OperatorType
      ,BlockFactory::EnumerationType
      ,BlockFactory::TypeListType
   };
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Namespace::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Namespace::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * This interface returns the real block children for this block. The real block 
 * children are all children of any access block children. If this is not a class 
 * then it is simply all direct children. 
 *
 * @return Pointer list of all real children for this block. 
 */
QList<AbstractBlock*> Namespace::realChildren() const
{
   return list();
}






/*!
 * Constructs a new namespace block with the given name. 
 *
 * @param name The value of this new namespace block's base name field. 
 */
Namespace::Namespace(const QString& name):
   Base(name)
{}






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get a root block pointer for this block's project tree, casting it to a 
 *    namespace type and saving it to _ret_. If the casting failed then throw an 
 *    exception. 
 *
 * 2. Return _ret_. 
 */
const Namespace* Namespace::root() const
{
   const Namespace* ret {qobject_cast<const Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get a root block pointer for this block's project tree, casting it to a 
 *    namespace type and saving it to _ret_. If the casting failed then throw an 
 *    exception. 
 *
 * 2. Return _ret_. 
 */
Namespace* Namespace::root()
{
   Namespace* ret {qobject_cast<Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Namespace::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}
