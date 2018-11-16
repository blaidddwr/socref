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
   explicit View(BasicBlock* block);
public slots:
   void update();
private:
   /*!
    */
   static const char* _header1Tag;
   /*!
    */
   static const char* _header2Tag;
   /*!
    */
   static const char* _header3Tag;
   /*!
    */
   static const char* _paragraphsTag;
   /*!
    */
   static const char* _customTag;
   /*!
    */
   static const char* _nameKey;
private:
   void addParagraphs(QString* text, const QDomElement& element);
   void addCustom(QString* text, const QDomElement& element);
   /*!
    */
   BasicBlock* _block;
};



#endif
