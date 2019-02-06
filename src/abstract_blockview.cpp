#include "abstract_blockview.h"



using namespace Abstract;
//






/*!
 * Constructs a new block view with the given block. 
 *
 * @param block The block that this view displays. 
 */
BlockView::BlockView(const Block* block):
   _block(block)
{
   Q_CHECK_PTR(block);
}






/*!
 * Called when this view's block's data has changed. This clears this view's text 
 * and calls the interface to render it again with the updated block. 
 */
void BlockView::blockUpdated()
{
   // Clear this view's text and render it again. 
   clear();
   renderText();
}
