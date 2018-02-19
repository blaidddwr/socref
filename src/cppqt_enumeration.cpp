#include <exception.h>
#include "cppqt_enumeration.h"
#include "cppqt_view_enumeration.h"
#include "cppqt_edit_enumeration.h"
#include "cppqt_blockfactory.h"
#include "cppqt_enumvalue.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Enumeration::_classTag {"class"};






Enumeration::Enumeration(const QString& name):
   Base(name)
{}






QString Enumeration::name() const
{
   QString ret {"enum "};
   if ( _class ) ret.append("class ");
   return ret.append(Base::name());
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
   static QList<int> ret;
   if ( ret.isEmpty() ) ret << BlockFactory::EnumValueType;
   return ret;
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
      notifyOfNameChange();
      emit modified();
   }
}






QList<EnumValue*> Enumeration::values() const
{
   return makeChildListOfType<EnumValue>(BlockFactory::EnumValueType);
}






void Enumeration::readData(const QDomElement& data)
{
   Base::readData(data);
   DomElementReader reader(data);
   _class = reader.attributeToInt(_classTag,false);
}






QDomElement Enumeration::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _class ) ret.setAttribute(_classTag,_class);
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
