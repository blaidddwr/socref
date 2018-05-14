#include "cppqt_enumeration.h"
#include <exception.h>
#include "cppqt_view_enumeration.h"
#include "cppqt_edit_enumeration.h"
#include "cppqt_blockfactory.h"
#include "cppqt_enumvalue.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Enumeration::_classTag {"class"};






Enumeration::Enumeration(const QString& name):
   Base(name)
{}






QString Enumeration::name() const
{
   QString ret {Base::name()};
   if ( _class ) ret.append(" [C]");
   return ret;
}






int Enumeration::type() const
{
   return BlockFactory::EnumerationType;
}






QIcon Enumeration::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumeration.svg");
   return ret;
}






QList<int> Enumeration::buildList() const
{
   return QList<int>{BlockFactory::EnumValueType};
}






std::unique_ptr<QWidget> Enumeration::makeView() const
{
   return unique_ptr<QWidget>(new View::Enumeration(this));
}






std::unique_ptr<::Gui::AbstractEdit> Enumeration::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Enumeration(this));
}






bool Enumeration::isClass() const
{
   return _class;
}






void Enumeration::setClass(bool isClass)
{
   if ( _class != isClass )
   {
      _class = isClass;
      notifyModified();
      notifyNameModified();
   }
}






QList<EnumValue*> Enumeration::values() const
{
   return makeListOfType<EnumValue>(BlockFactory::EnumValueType);
}






void Enumeration::readData(const QDomElement& data, int version)
{
   Base::readData(data,version);
   switch (version)
   {
   case 0:
      readVersion0(data);
      break;
   case 1:
      readVersion1(data);
      break;
   default:
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Unknown version number %1 given for reading block.").arg(version));
         throw e;
      }
   }
}






int Enumeration::writeVersion() const
{
   return _verison;
}






QDomElement Enumeration::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _class ) ret.appendChild(document.createElement(_classTag));
   return ret;
}






std::unique_ptr<AbstractBlock> Enumeration::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Enumeration);
}






void Enumeration::copyDataFrom(const AbstractBlock* object)
{
   if ( const Enumeration* object_ = qobject_cast<const Enumeration*>(object) )
   {
      Base::copyDataFrom(object);
      _class = object_->_class;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






void Enumeration::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _class = reader.attributeToInt(_classTag,false);
}






void Enumeration::readVersion1(const QDomElement& data)
{
   DomElementReader reader(data);
   reader.set(_classTag,&_class,false);
   reader.read();
}
