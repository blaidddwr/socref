#include <QDomNode>
#include <QDomElement>
#include "domelementreader.h"
#include "exception.h"






DomElementReader::DomElementReader(const QDomElement& element):
   _element(element)
{}






DomElementReader& DomElementReader::set(const QString& tagName, QString* pointer, bool onlyOnce, bool required)
{
   return append(tagName,pointer,Type::String,onlyOnce,required);
}






DomElementReader& DomElementReader::set(const QString& tagName, int* pointer, bool onlyOnce, bool required)
{
   return append(tagName,pointer,Type::Number,onlyOnce,required);
}






DomElementReader& DomElementReader::set(const QString& tagName, QDomElement* pointer, bool onlyOnce, bool required)
{
   return append(tagName,pointer,Type::Element,onlyOnce,required);
}






QString DomElementReader::attribute(const QString& name, bool required)
{
   if ( !_element.hasAttribute(name) )
   {
      if ( required )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot find attribute '%1' in XML element '%2'.").arg(name).arg(_element.tagName()));
         throw e;
      }
      return QString();
   }
   return _element.attribute(name);
}






int DomElementReader::attributeToInt(const QString& name, bool required)
{
   if ( !_element.hasAttribute(name) )
   {
      if ( required )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot find attribute '%1' in XML element '%2'.").arg(name).arg(_element.tagName()));
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
      e.setDetails(QObject::tr("Failed reading attribute '%1' in XML element '%2' as integer.").arg(name).arg(_element.tagName()));
      throw e;
   }
   return ret;
}






void DomElementReader::read()
{
   QDomNode node {_element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         const QHash<QString,int>& lookup {_lookup};
         auto i {lookup.find(element.tagName())};
         if ( i != lookup.end() && ( !_read.at(*i) || !_onlyOnce.at(*i) ) )
         {
            switch (_type.at(*i))
            {
            case Type::String:
               *static_cast<QString*>(_data[*i]) = element.text();
               break;
            case Type::Number:
               {
                  bool ok;
                  *static_cast<int*>(_data[*i]) = element.text().toInt(&ok);
                  if ( !ok )
                  {
                     Exception::ReadError e;
                     MARK_EXCEPTION(e);
                     e.setDetails(QObject::tr("Failed reading XML element '%1' as integer.").arg(element.tagName()));
                     throw e;
                  }
                  break;
               }
            case Type::Element:
               *static_cast<QDomElement*>(_data[*i]) = element;
               break;
            }
            _read[*i] = true;
         }
      }
      node = node.nextSibling();
   }
}






bool DomElementReader::allRequiredFound() const
{
   return !_read.contains(false);
}






DomElementReader& DomElementReader::append(const QString& tagName, void* pointer, Type type, bool onlyOnce, bool required)
{
   if ( _lookup.contains(tagName) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Tag name '%1' already exists in list of elements to find.").arg(tagName));
      throw e;
   }
   _lookup.insert(tagName,_type.size());
   _type.append(type);
   _data.append(static_cast<void*>(pointer));
   _read.append(!required);
   _onlyOnce.append(onlyOnce);
   return *this;
}
