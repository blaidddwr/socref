#include <QDomElement>
#include "cppqt_type_template.h"
#include "exception.h"
#include "cppqt_type_factory.h"



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






QString Template::fullName() const
{
   QString ret {name()};
   if ( !_variants.isEmpty() )
   {
      ret.append("<");
      for (int i = 0; i < _variants.size() ;++i)
      {
         if ( _values.at(i) )
         {
            ret.append(_values.at(i)->fullName());
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






AbstractType* Template::read(const QDomElement& type)
{
   enum
   {
      Variant = 0
      ,Value
      ,Total
   };
   QStringList tags {"variant","value"};
   if ( !type.hasAttribute("name") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("C++/Qt template type missing name attribute."));
      throw e;
   }
   setName(type.attribute("name"));
   QDomNode node = type.firstChild();
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (tags.indexOf(element.tagName()))
         {
         case Variant:
            _variants.push_back(element.text());
            break;
         case Value:
            _values.push_back(readValue(element));
            break;
         }
      }
      node = node.nextSibling();
   }
   if ( _variants.size() != _values.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Corrupt template element; variant and value size does not match."));
      throw e;
   }
   return this;
}






int Template::type() const
{
   return Type::Factory::TemplateType;
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






QDomElement Template::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.setAttribute("name",name());
   for (const auto variant : _variants)
   {
      QDomElement variant_ {document.createElement("variant")};
      variant_.appendChild(document.createTextNode(variant));
      ret.appendChild(variant_);
   }
   for (auto value : _values)
   {
      if ( value )
      {
         QDomElement value_ {value->write(document)};
         value_.setTagName("value");
         ret.appendChild(value_);
      }
      else
      {
         ret.appendChild(document.createElement("value"));
      }
   }
   return ret;
}






AbstractType* Template::readValue(const QDomElement& value)
{
   Type::Factory& factory {Type::Factory::instance()};
   if ( factory.isValidTypeElement(value) )
   {
      return Type::Factory::instance().read(value).release();
   }
   return nullptr;
}
