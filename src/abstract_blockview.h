#ifndef ABSTRACT_BLOCKVIEW_H
#define ABSTRACT_BLOCKVIEW_H
#include <QLabel>
#include "abstract.h"



/*!
 * Contains all abstract interface classes that define the meta structure of
 * this application.
 */
namespace Abstract
{
   /*!
    * This represents a label that displays the data of a block. This connects to
    * the block it represents and updates its label whenever its block updates
    * itself. This provides a single virtual method that should render the text of
    * its label, displaying all data of its connected block. The block is read only
    * because this is only meant for displaying its data.
    */
   class BlockView : public QLabel
   {
      Q_OBJECT
   public:
      explicit BlockView(const Block* block);
   public slots:
      void blockUpdated();
   protected:
      /*!
       * This interface renders the text onto this label displaying all data of the
       * block this view displays.
       */
      virtual void renderText() = 0;
   protected:
      template<class T> const T& block();
   private:
      /*!
       * The block that this view displays.
       */
      const Block* _block;
   };
}






namespace Abstract
{
   /*!
    * Returns a reference to the block this view displays, type casted to the
    * specific block type of this view. It is assumed the type given can be type
    * cast successfully.
    *
    * @tparam T Block type to return as a reference.
    *
    * @return Reference to the block this view displays.
    */
   template<class T> const T& BlockView::block()
   {
      // Type cast this view's block to the given block type and make sure it worked.
      const T* ret {qobject_cast<const T*>(_block)};
      Q_ASSERT(ret);

      // Return a reference to this view's type casted block.
      return *ret;
   }
}

#endif
