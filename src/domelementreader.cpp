#include "domelementreader.h"
#include <QDomNode>
#include <QDomElement>
#include <QObject>
#include <exception.h>
#include "domelementreader_match.h"
//






/*!
 * Constructs a new element reader with the given XML element that this object 
 * can read. 
 *
 * @param element The XML element this object will read. 
 */
DomElementReader::DomElementReader(const QDomElement& element):
   _element(element)
{}






/*!
 * Deletes all allocates matches in the lookup list. 
 */
DomElementReader::~DomElementReader()
{
   qDeleteAll(_lookup);
}






/*!
 * Returns the text value of the given attribute from this reader's element. If 
 * no attribute exists with the given name and it is required then an exception 
 * is thrown. 
 *
 * @param name Name of attribute whose value is returned. 
 *
 * @param required True if an exception should be thrown if the attribute is not 
 *                 found otherwise false. 
 *
 * @return Value of the attribute with the given name. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the attribute is not found and it is required then throw an exception, 
 *    else if the attribute is not found and it is not required then return an 
 *    empty string, else return the value of the attribute. 
 */
QString DomElementReader::attribute(const QString& name, bool required) const
{
   if ( !_element.hasAttribute(name) )
   {
      if ( required )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(
                  QObject::tr("Cannot find attribute '%1' in XML element '%2'.")
                  .arg(name)
                  .arg(_element.tagName()));
         throw e;
      }
      return QString();
   }
   return _element.attribute(name);
}






/*!
 * Returns the integer value of the given attribute from this reader's element. 
 * If no attribute exists with the given name or the value is not an integer, 
 * and the attribute is required, then an exception is thrown. 
 *
 * @param name Name of attribute whose integer value is returned. 
 *
 * @param required True if an exception should be thrown if the attribute is not 
 *                 found or failed to read as an integer otherwise false. 
 *
 * @return Value of the attribute with the given name. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the attribute is not found and it is required then throw an exception, 
 *    else if the attribute is not found and it is not required then return 0, 
 *    else go to the next step. 
 *
 * 2. Read in the attributes value as an integer setting it to 0 if it fails to 
 *    read as an integer. If the value is not an integer and it is required then 
 *    throw an exception. 
 *
 * 3. Return the integer value of the attribute. 
 */
int DomElementReader::attributeToInt(const QString& name, bool required) const
{
   if ( !_element.hasAttribute(name) )
   {
      if ( required )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(
                  QObject::tr("Cannot find attribute '%1' in XML element '%2'.")
                  .arg(name)
                  .arg(_element.tagName()));
         throw e;
      }
      return 0;
   }
   bool ok;
   int ret {_element.attribute(name).toInt(&ok)};
   if ( !ok && required )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               QObject::tr("Failed reading attribute '%1' in XML element '%2' as integer.")
               .arg(name)
               .arg(_element.tagName()));
      throw e;
   }
   return ret;
}






/*!
 * Set a new child element match with the given tag name. When this reader reads 
 * its element, if a child element is found with the given tag name then the 
 * string pointed to by the given pointer will be set to the matched child 
 * element's text. 
 *
 * @param tagName Tag name to find in this reader's child elements. 
 *
 * @param pointer Pointer to a string that will be set if a child element is 
 *                matched. 
 *
 * @param required True if this match is required otherwise false. 
 *
 * @return Reference to this element reader. 
 */
DomElementReader& DomElementReader::set(const QString& tagName, QString* pointer, bool required)
{
   return append(tagName,pointer,Type::String,required);
}






/*!
 * Set a new child element match with the given tag name. When this reader reads 
 * its element, if a child element is found with the given tag name then the 
 * integer pointed to by the given pointer will be set to the matched child 
 * element's text. If the matched element's text cannot be converted to an 
 * integer then an exception will be thrown. 
 *
 * @param tagName Tag name to find in this reader's child elements. 
 *
 * @param pointer Pointer to an integer that will be set if a child element is 
 *                matched. 
 *
 * @param required True if this match is required otherwise false. 
 *
 * @return Reference to this element reader. 
 */
DomElementReader& DomElementReader::set(const QString& tagName, int* pointer, bool required)
{
   return append(tagName,pointer,Type::Number,required);
}






/*!
 * Set a new child element match with the given tag name. When this reader reads 
 * its element, if a child element is found with the given tag name then the 
 * boolean pointed to by the given pointer will be set to true. 
 *
 * @param tagName Tag name to find in this reader's child elements. 
 *
 * @param pointer Pointer to a boolean that will be set to true if a child 
 *                element is matched. 
 *
 * @param required True if this match is required otherwise false. 
 *
 * @return Reference to this element reader. 
 */
DomElementReader& DomElementReader::set(const QString& tagName, bool* pointer, bool required)
{
   return append(tagName,pointer,Type::Boolean,required);
}






/*!
 * Set a new child element match with the given tag name. When this reader reads 
 * its element, if a child element is found with the given tag name then the XML 
 * element pointed to by the given pointer will be set to the matched child 
 * element. 
 *
 * @param tagName Tag name to find in this reader's child elements. 
 *
 * @param pointer Pointer to a XML element that will be set to a child element 
 *                is matched. 
 *
 * @param required True if this match is required otherwise false. 
 *
 * @return Reference to this element reader. 
 */
DomElementReader& DomElementReader::set(const QString& tagName, QDomElement* pointer, bool required)
{
   return append(tagName,pointer,Type::Element,required);
}






/*!
 * Set a new child element match with the given tag name. When this reader reads 
 * its element, if a child element is found with the given tag name then it is 
 * appended to the list of elements pointed to by the match. 
 *
 * @param tagName Tag name to find in this reader's child elements. 
 *
 * @param pointer Pointer to a list of XML elements that will have any matched 
 *                child elements appended to it. 
 *
 * @param required True if this match is required otherwise false. 
 *
 * @return Reference to this element reader. 
 */
DomElementReader& DomElementReader::set(const QString& tagName, QList<QDomElement>* pointer, bool required)
{
   return append(tagName,pointer,Type::ElementList,required);
}






/*!
 * Reads this reader's XML element for child elements, finding any matches set 
 * before this is called. Before reading begins this clears all variables 
 * pointed to by the list of matches set before this is called. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Clear all variables pointed to by this reader's list of matches and 
 *    iterate through all child elements of this reader's XML element for the 
 *    proceeding steps. 
 *
 * 2. If the child element matches one of this reader's matches then go to the 
 *    next step. 
 *
 * 3. If the match type is a string then set the string to the child element's 
 *    text, else if the match type is a number then set the integer to the child 
 *    element's text throwing an exception if it is not an integer, else if the 
 *    match type is a boolean then set the boolean to true, else if the match 
 *    type is an element then set the element to the child element, else if the 
 *    match is an element list then append the child element to the list. 
 */
void DomElementReader::read()
{
   clear();
   QDomNode node {_element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         auto i {qAsConst(_lookup).find(element.tagName())};
         if ( i != _lookup.end() )
         {
            Match* match {*i};
            switch (match->_type)
            {
            case Type::String:
               *static_cast<QString*>(match->_data) = element.text();
               break;
            case Type::Number:
               {
                  bool ok;
                  *static_cast<int*>(match->_data) = element.text().toInt(&ok);
                  if ( !ok )
                  {
                     Exception::ReadError e;
                     MARK_EXCEPTION(e);
                     e.setDetails(
                              QObject::tr("Failed reading XML element '%1' as integer.")
                              .arg(element.tagName()));
                     throw e;
                  }
                  break;
               }
            case Type::Boolean:
               *static_cast<bool*>(match->_data) = true;
               break;
            case Type::Element:
               *static_cast<QDomElement*>(match->_data) = element;
               break;
            case Type::ElementList:
               static_cast<QList<QDomElement>*>(match->_data)->append(element);
               break;
            }
            match->_read = true;
         }
      }
      node = node.nextSibling();
   }
}






/*!
 * Tests if all required matches found at least one match after called the read 
 * method. 
 *
 * @return True if all required matches were found or false otherwise. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all matches. If a match is found that was not read then 
 *    return false, else if all matches were read then return true. 
 */
bool DomElementReader::allRequiredFound() const
{
   for (auto match : qAsConst(_lookup))
   {
      if ( !match->_read ) return false;
   }
   return true;
}






/*!
 * Inserts a new match to the lookup hash table of matches for this element 
 * reader. If a tag name already exists then an exception is thrown. 
 *
 * @param tagName Tag name for the new match that is inserted. 
 *
 * @param pointer Pointer to the variable that is used for the given match that 
 *                is inserted. 
 *
 * @param type The type for the given match that is inserted. 
 *
 * @param required True if this match is required or false otherwise. 
 *
 * @return Reference to this element reader. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If a match already exists in this element reader's lookup hash table with 
 *    the given tag name then throw an exception, else go to the next step. 
 *
 * 2. Add a new match to this element reader's lookup hash table using the given 
 *    tag name as the key. Use the inverse of the required condition for the new 
 *    match's read condition which marks any matches that are not required as 
 *    read. 
 */
DomElementReader& DomElementReader::append(const QString& tagName, void* pointer, Type type, bool required)
{
   if ( _lookup.contains(tagName) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               QObject::tr("Tag name '%1' already exists in list of elements to find.")
               .arg(tagName));
      throw e;
   }
   _lookup.insert(tagName,new Match {type,pointer,!required});
   return *this;
}






/*!
 * Clears the variables of all matches to default values. For integers that is 0 
 * and for boolean that is false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all matches of this element reader, setting the variables 
 *    each for each match to its default value. For strings that is an empty 
 *    string, for numbers that is 0, for boolean that is false, for element that 
 *    is calling its clear method, and for element lists that is also calling 
 *    its clear method. 
 */
void DomElementReader::clear()
{
   for (auto match : qAsConst(_lookup))
   {
      switch (match->_type)
      {
      case Type::String:
         static_cast<QString*>(match->_data)->clear();
         break;
      case Type::Number:
         *static_cast<int*>(match->_data) = 0;
         break;
      case Type::Boolean:
         *static_cast<bool*>(match->_data) = false;
         break;
      case Type::Element:
         static_cast<QDomElement*>(match->_data)->clear();
         break;
      case Type::ElementList:
         static_cast<QList<QDomElement>*>(match->_data)->clear();
         break;
      }
   }
}
