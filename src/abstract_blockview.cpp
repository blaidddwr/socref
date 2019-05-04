#include "abstract_blockview.h"
#include "abstract_block.h"



namespace Abstract
{






/*!
 * Constructs a new block view with the given block.
 *
 * @param block The block that this view displays.
 */
BlockView::BlockView(const Block* block)
   :
   _block(block)
{
   // Make sure the block pointer is valid and connect the updated signal.
   Q_ASSERT(block);
   connect(block,&Block::updated,this,&BlockView::blockUpdated);
}






/*!
 * Called when this view's block's data has changed. This clears this view's
 * text and calls the interface to render it again with the updated block.
 */
void BlockView::blockUpdated()
{
   // Clear this view's text and render it again.
   clear();
   renderText();
}

}
