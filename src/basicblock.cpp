#include "basicblock.h"



//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int BasicBlock::type() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& BasicBlock::factory() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString BasicBlock::name() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon BasicBlock::icon() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> BasicBlock::buildList() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> BasicBlock::makeView() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> BasicBlock::makeEdit()
{}






/*!
 *
 * @param element  
 *
 * @param _buildList  
 *
 * @param isDefault  
 */
void BasicBlock::initialize(const QDomElement& element, const QList<int>& _buildList, bool isDefault)
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param element The XML element used to read in this blocks data. 
 */
void BasicBlock::readData(const QDomElement& element)
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param document XML document to use for creating new elements. 
 *
 * @return See interface docs. 
 */
QDomElement BasicBlock::writeData(QDomDocument& document) const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlock::makeBlank() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param other The other block whose data will be copied. 
 */
void BasicBlock::copyDataFrom(const AbstractBlock* other)
{}
