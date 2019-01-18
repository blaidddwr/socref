#ifndef BASICBLOCK_VIEW_H
#define BASICBLOCK_VIEW_H
#include "basicblock.h"
#include <QLabel>
//



/*!
 * This is the basic block view that displays an associated basic block. Like the 
 * basic block it views this class reads in its configuration from an XML element 
 * supplied to it from its basic block. The XML definition provides all information 
 * about the layout of display elements this view displays. This class can be 
 * implemented by another more specialized class and is necessary to do so if the 
 * custom viewing element is used. 
 */
class BasicBlock::View : public QLabel
{
   Q_OBJECT
public:
   explicit View(const BasicBlock* block);
public slots:
   void update();
private:
   /*!
    * The tag name for a header 1 display element, 1 being the largest. 
    */
   static const char* _header1Tag;
   /*!
    * The tag name for a header 2 display element, 1 being the largest. 
    */
   static const char* _header2Tag;
   /*!
    * The tag name for a header 3 display element, 1 being the largest. 
    */
   static const char* _header3Tag;
   /*!
    * The tag name for a paragraphs display element that displays a basic block field 
    * as multiple paragraphs separated by double newline characters. 
    */
   static const char* _paragraphsTag;
   /*!
    * The tag name for a custom display element specifying a custom method to call 
    * that returned a custom string. 
    */
   static const char* _customTag;
   /*!
    * The attribute name for the custom method name of a custom display element. 
    */
   static const char* _nameKey;
private:
   void addParagraphs(QString* text, const QDomElement& element);
   void addCustom(QString* text, const QDomElement& element);
   QStringList parseBoldMarkers(const QStringList& list);
   /*!
    * The basic block that this view is displaying. 
    */
   const BasicBlock* _block;
};



#endif
