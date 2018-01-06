#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "cppqt_type_template.h"
#include "exception.h"
#include "xmlelementparser.h"
#include "cppqt_typefactory.h"



using namespace std;
using namespace CppQt;
using namespace CppQt::Type;






Template::Template(const QString& name, const QList<QString>& variants):
   AbstractType(name),
   _variants(variants)
{
   while ( _variants.size() > _values.size() )
   {
      _values.push_back(nullptr);
   }
}






bool Template::isConcrete() const
{
   for (auto value : _values)
   {
      if ( !value )
      {
         return false;
      }
      if ( !value->isConcrete() )
      {
         return false;
      }
   }
   return true;
}






bool Template::isEquivalent(const AbstractType* type) const
{
   if ( const Template* type_ = dynamic_cast<const Template*>(type) )
   {
      return name() == type_->name() && _variants == type_->_variants;
   }
   return false;
}






QString Template::fullName(const QList<QString>& scope) const
{
   QString ret {name()};
   if ( !_variants.isEmpty() )
   {
      ret.append("<");
      for (int i = 0; i < _variants.size() ;++i)
      {
         if ( _values.at(i) )
         {
            ret.append(_values.at(i)->fullName(scope));
         }
         else
         {
            ret.append(_variants.at(i));
         }
         if ( i != (_variants.size() - 1) )
         {
            ret.append(",");
         }
      }
      ret.append(">");
   }
   return ret;
}






AbstractType* Template::read(QXmlStreamReader& xml)
{
   enum
   {
      Name = 0
      ,Variant
      ,Value
   };
   XMLElementParser parser(xml);
   parser.addKeyword("name",true).addKeyword("variant",false,true).addKeyword("value",false,true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Name:
         setName(xml.readElementText());
         break;
      case Variant:
         _variants.push_back(xml.readElementText());
         break;
      case Value:
         _values.push_back(TypeFactory::instance().read(xml).release());
         break;
      }
   }
   if ( _variants.size() != _values.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Corrupt template element; variant and value size does not match."));
      throw e;
   }
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Failed reading in all required elements."));
      throw e;
   }
   return this;
}






int Template::type() const
{
   return TypeFactory::TemplateType;
}






Template& Template::setVariants(const QList<QString>& variants)
{
   _variants = variants;
   while ( _variants.size() > _values.size() )
   {
      _values.push_back(nullptr);
   }
   while ( _values.size() > _variants.size() )
   {
      delete _values.back();
      _values.pop_back();
   }
   return *this;
}






Template& Template::setValue(int index, unique_ptr<CppQt::AbstractType>&& type)
{
   if ( index >= _variants.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set value of index %1 when template only holds %2 variants.").arg(index).arg(_variants.size()));
      throw e;
   }
   delete _values.at(index);
   _values[index] = type.release();
   return *this;
}






Template& Template::clearValues()
{
   qDeleteAll(_values);
   for (auto& value : _values)
   {
      value = nullptr;
   }
   return *this;
}






void Template::writeData(QXmlStreamWriter& xml) const
{
   xml.writeTextElement("name",name());
   for (const auto variant : _variants)
   {
      xml.writeTextElement("variant",variant);
   }
   for (auto value : _values)
   {
      value->write("value",xml);
   }
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Xml Error writing to file."));
      throw e;
   }
}
