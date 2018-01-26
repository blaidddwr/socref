#include "domelementreader.h"






DomElementReader::DomElementReader(const QDomNode& node):
   _node(node)
{}






DomElementReader& DomElementReader::set(const QString& tagName, QString* pointer, bool onlyOnce, bool required)
{
   append(tagName,static_cast<void*>(pointer),Type::String,onlyOnce,required);
}






DomElementReader& DomElementReader::set(const QString& tagName, int* pointer, bool onlyOnce, bool required)
{
}






DomElementReader& DomElementReader::find(const QString& tagName, int slot, bool onlyOnce, bool required)
{
}






void DomElementReader::read()
{
}






bool DomElementReader::allRequiredFound() const
{
}






QDomElement *DomElementReader::get(int index) const
{
}






DomElementReader& DomElementReader::append(const QString& tagName, void* pointer, Type type, bool onlyOnce, bool required)
{
   if ( _lookup.contains(tagName) )
   {
      ;//ERROR
   }
   _lookup.insert(tagName,_type.size());
   _type.append(type);
   _data.append(static_cast<void*>(pointer));
   _read.append(!required);
   _onlyOnce.append(onlyOnce);
}
