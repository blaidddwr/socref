#include "cppqt_destructor.h"
#include "cppqt_destructor_edit.h"
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
int Destructor::type() const
{
   return BlockFactory::DestructorType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Destructor::name() const
{
   // Return the constructor name with a tilde at the beginning. 
   return QString("~").append(Constructor::name());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Destructor::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon regular(":/icons/destructor.svg");
   static QIcon virtual_(":/icons/vdestructor.svg");
   static QIcon abstract(":/icons/adestructor.svg");

   // Return the appropriate icon based off this destructor block's properties. 
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Destructor::buildList() const
{
   return QList<int>();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Destructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Destructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Destructor);
}
