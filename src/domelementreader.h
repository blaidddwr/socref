#ifndef DOMELEMENTREADER_H
#define DOMELEMENTREADER_H
#include <QString>
#include <QHash>
#include <QPair>



class QDomElement;
//



/*!
 * This is a helper class that makes is easier to read information stored in an 
 * XML element. This primarily helps with finding child elements and extracting 
 * information enclosed within them. This also helps with getting attributes in 
 * text or integer form and throwing exceptions if they don't exist. For 
 * searching child elements this class works by adding matches that will be 
 * looked for when the XML element given to this class is read. When a match is 
 * found the variable passed to the match is set to the value of the matched 
 * child element. This class is designed to only read its given XML element once 
 * and then be destroyed. 
 */
class DomElementReader
{
public:
   DomElementReader(const QDomElement& element);
   ~DomElementReader();
   QString attribute(const QString& name, bool required = true) const;
   int attributeToInt(const QString& name, bool required = true) const;
   DomElementReader& set(const QString& tagName, QString* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, int* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, bool* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, QDomElement* pointer, bool required = true);
   DomElementReader& set(const QString& tagName, QList<QDomElement>* pointer, bool required = true);
   void read();
   bool allRequiredFound() const;
private:
   class Match;
   /*!
    * Defines all the possible match types that can be set in an element reader. 
    */
   enum class Type
   {
      /*!
       * Defines a match that sets a string. 
       */
      String
      /*!
       * Defines a match that sets an integer. 
       */
      ,Number
      /*!
       * Defines a match that sets a boolean. 
       */
      ,Boolean
      /*!
       * Defines a match that sets another element. 
       */
      ,Element
      /*!
       * Defines a match that adds to an element list. 
       */
      ,ElementList
   };
   DomElementReader& append(const QString& tagName, void* pointer, Type type, bool required);
   void clear();
   /*!
    * The XML element that this element reader reads the direct children of with 
    * the read method and extracts arguments from. 
    */
   const QDomElement& _element;
   /*!
    * The hash table used to store all matches and look them up by their tag name 
    * for this element reader used when reading the child elements of this reader's 
    * XML element. 
    */
   QHash<QString,Match*> _lookup;
};



#endif
