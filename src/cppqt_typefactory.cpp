#include <QXmlStreamReader>
#include <QDomElement>

#include "cppqt_typefactory.h"
#include "cppqt_type_concrete.h"
#include "cppqt_type_template.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






unique_ptr<AbstractType> TypeFactory::read(const QDomElement& type) const
{
   if ( !type.hasAttribute("id") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Type element missing id attribute."));
      throw e;
   }
   bool ok;
   int type_ {type.attribute("id").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Failed reading in type attribute."));
      throw e;
   }
   if ( type_ < 0 || type_ >= Total )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Read in invalid type %1 when max is %2.").arg(type_).arg(Total));
      throw e;
   }
   unique_ptr<AbstractType> ret {makeType(type_)};
   ret->read(type);
   ret->setScope(type.attribute("scope").split("::"));
   ret->setDepth(type.attribute("depth").toInt());
   return ret;
}






bool TypeFactory::isValidTypeElement(const QDomElement& element)
{
   return element.hasAttribute("id") && element.hasAttribute("type");
}






QString TypeFactory::name(int type) const
{
   switch (type)
   {
   case ConcreteType:
      return "concrete";
   case TemplateType:
      return "template";
   default:
      return "unkonwn";
   }
}






unique_ptr<AbstractType> TypeFactory::makeType(int type) const
{
   switch (type)
   {
   case ConcreteType:
      return unique_ptr<AbstractType>(new Type::Concrete);
   case TemplateType:
      return unique_ptr<AbstractType>(new Type::Template);
   default:
      return nullptr;
   }
}
