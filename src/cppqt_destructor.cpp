#include "cppqt_destructor.h"
#include "cppqt_view_destructor.h"
#include "cppqt_edit_destructor.h"
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
int Destructor::type() const
{
   return BlockFactory::DestructorType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Destructor::name() const
{
   return QString("~").append(Constructor::name());
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Destructor::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/destructor.svg");
      virtual_ = QIcon(":/icons/vdestructor.svg");
      abstract = QIcon(":/icons/adestructor.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block type can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Destructor::buildList() const
{
   return QList<int>();
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Destructor::makeView() const
{
   return unique_ptr<QWidget>(new View::Destructor(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Destructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Destructor(this));
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Destructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Destructor);
}
