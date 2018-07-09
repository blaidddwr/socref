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
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Namespace::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/namespace.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Namespace::buildList() const
{
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
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> Namespace::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
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
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 */
const Namespace* Namespace::root() const
{
   // Get the root pointer for this block, casting it as a namespace block and making 
   // sure it worked. 
   const Namespace* ret {qobject_cast<const Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }

   // Return the root namespace pointer. 
   return ret;
}






/*!
 * Returns a pointer to the root namespace of this block's entire project. 
 *
 * @return Pointer to the root namespace of this block's project. 
 */
Namespace* Namespace::root()
{
   // Get the root pointer for this block, casting it as a namespace block and making 
   // sure it worked. 
   Namespace* ret {qobject_cast<Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }

   // Return the root namespace pointer. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Namespace::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}
