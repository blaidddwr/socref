#include "basicblockfactory.h"
#include <socutil/sut_exceptions.h>



//






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int BasicBlockFactory::size() const
{
   return _typeNames.size();
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BasicBlockFactory::name(int type) const
{
   if ( type < 0 || type >= _typeNames.size() )
   {
      Sut::Exception::OutOfRange e;
      e.setDetails(QObject::tr("Given block type %1 is out of range."));
      throw e;
   }
   return _typeNames.at(type);
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param elementName See interface docs. 
 *
 * @return See interface docs. 
 */
int BasicBlockFactory::typeByElementName(const QString& elementName) const
{
   return _typeElementNames.indexOf(elementName);
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BasicBlockFactory::elementName(int type) const
{
   if ( type < 0 || type >= _typeElementNames.size() )
   {
      Sut::Exception::OutOfRange e;
      e.setDetails(QObject::tr("Given block type %1 is out of range."));
      throw e;
   }
   return _typeElementNames.at(type);
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @param isDefault See interface docs. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlockFactory::makeBlock(int type, bool isDefault) const
{
   // .
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlockFactory::makeRootBlock() const
{
   // .
}






/*!
 *
 * @param xmlPath  
 *
 * @param elementNames  
 */
BasicBlockFactory::BasicBlockFactory(const QString& xmlPath, const QStringList& elementNames)
{}






/*!
 */
BasicBlockFactory::~BasicBlockFactory()
{
   qDeleteAll(_typeDefinitions);
}
