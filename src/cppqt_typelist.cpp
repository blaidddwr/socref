#include "cppqt_typelist.h"
#include "cppqt_typelist_view.h"
#include "cppqt_typelist_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int TypeList::type() const
{
   return BlockFactory::TypeListType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString TypeList::name() const
{
   // Return the base name appended with the number of children this block contains. 
   return Base::name().append(" (").append(QString::number(size())).append(")");
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon TypeList::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/typelist.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> TypeList::buildList() const
{
   return {BlockFactory::TypeListType,BlockFactory::TypeType};
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<QWidget> TypeList::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> TypeList::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> TypeList::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new TypeList);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childNameModified(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the body has changed and return false. 
   notifyBodyModified();
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childAdded(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body has changed and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::childRemoved(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body has changed and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @param value See interface docs. 
 *
 * @return See interface docs. 
 */
bool TypeList::checkName(const QString& value)
{
   // Use a regular expression to determine if the given value is valid. 
   return QRegExp("[a-zA-Z_]+((::)?[a-zA-Z0-9_])*").exactMatch(value);
}
