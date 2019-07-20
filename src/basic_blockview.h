#ifndef BASIC_BLOCKVIEW_H
#define BASIC_BLOCKVIEW_H
#include "abstract_blockview.h"
#include "basic.h"
class QDomElement;



/*!
 * This contains all classes that implement the block and parsing system with a
 * basic foundation that can be used by specific project types.
 */
namespace Basic
{
   /*!
    * This implements the abstract basic block view that displays an associated
    * basic block. Like the basic block it views, this class reads in its
    * configuration from an XML element supplied to it from its basic block. The
    * XML definition provides all information about the layout of display elements
    * this view displays. This class can be implemented by another more specialized
    * class and is necessary to do so if the custom viewing element is used.
    */
   class BlockView : public Abstract::BlockView
   {
      Q_OBJECT
   public:
      explicit BlockView(const Block* block);
   protected:
      virtual void renderText() override final;
   private:
      static QStringList parseBoldMarkers(const QStringList& list);
   private:
      /*!
       * The tag name for a header 1 display element, 1 being the largest.
       */
      static const QString _header1Tag;
      /*!
       * The tag name for a header 2 display element, 1 being the largest.
       */
      static const QString _header2Tag;
      /*!
       * The tag name for a header 3 display element, 1 being the largest.
       */
      static const QString _header3Tag;
      /*!
       * The tag name for a paragraphs display element that displays a basic block
       * field as multiple paragraphs separated by double newline characters.
       */
      static const QString _paragraphsTag;
      /*!
       * The tag name for a custom display element specifying a custom method to call
       * that returned a custom string.
       */
      static const QString _customTag;
      /*!
       * The attribute name for the custom method name of a custom display element.
       */
      static const QString _nameKey;
      /*!
       * The attribute name for the underline option for paragraph elements to make
       * text bold between underlines or ignore them.
       */
      static const QString _underlineKey;
   private:
      void addParagraphs(QString* text, const QDomElement& element);
      void addCustom(QString* text, const QDomElement& element);
   };
}

#endif
