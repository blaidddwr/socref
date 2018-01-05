#include <QXmlStreamReader>

#include "cppqt_typefactory.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






unique_ptr<AbstractType> TypeFactory::read(QXmlStreamReader& xml) const
{
   if ( !xml.attributes().hasAttribute("type") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Type element missing type attribute."));
      throw e;
   }
   bool ok;
   int type {xml.attributes().value("type").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Failed reading in type attribute."));
      throw e;
   }
   if ( type < 0 || type >= Total )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Read in invalid type %1 when max is %2.").arg(type).arg(Total));
      throw e;
   }
   unique_ptr<AbstractType> ret {makeType(type)};
   ret->read(xml);
   return ret;
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
