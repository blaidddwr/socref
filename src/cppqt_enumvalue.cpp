#include "cppqt_enumvalue.h"
#include <exception.h>
#include "cppqt_view_enumvalue.h"
#include "cppqt_edit_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* EnumValue::_valueTag {"value"};






EnumValue::EnumValue(const QString& name):
   Base(name)
{}






QString EnumValue::name() const
{
   QString ret {Base::name()};
   if ( _hasValue ) ret.append(" = ").append(QString::number(_value));
   return ret;
}






int EnumValue::type() const
{
   return BlockFactory::EnumValueType;
}






QIcon EnumValue::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/enumvalue.svg");
   return ret;
}






QList<int> EnumValue::buildList() const
{
   return QList<int>();
}






std::unique_ptr<QWidget> EnumValue::makeView() const
{
   return unique_ptr<QWidget>(new View::EnumValue(this));
}






std::unique_ptr<Gui::AbstractEdit> EnumValue::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::EnumValue(this));
}






bool EnumValue::hasValue() const
{
   return _hasValue;
}






int EnumValue::value() const
{
   return _value;
}






void EnumValue::clearValue()
{
   if ( _hasValue )
   {
      _hasValue = false;
      notifyOfNameChange();
      emit modified();
   }
}






void EnumValue::setValue(int value)
{
   if ( !_hasValue || _value != value )
   {
      _hasValue = true;
      _value = value;
      notifyOfNameChange();
      emit modified();
   }
}






void EnumValue::readData(const QDomElement& data, int version)
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






int EnumValue::writeVersion() const
{
   return _version;
}






QDomElement EnumValue::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _hasValue ) ret.appendChild(makeElement(document,_valueTag,QString::number(_value)));
   return ret;
}






std::unique_ptr<AbstractBlock> EnumValue::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new EnumValue);
}






void EnumValue::copyDataFrom(const AbstractBlock* object)
{
   if ( const EnumValue* object_ = qobject_cast<const EnumValue*>(object) )
   {
      Base::copyDataFrom(object);
      _hasValue = object_->_hasValue;
      _value = object_->_value;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






void EnumValue::readVersion0(const QDomElement& data)
{
   if ( data.hasAttribute(_valueTag) )
   {
      _hasValue = true;
      DomElementReader reader(data);
      _value = reader.attributeToInt(_valueTag);
   }
   else _hasValue = false;
}






void EnumValue::readVersion1(const QDomElement& data)
{
   QDomElement value;
   DomElementReader reader(data);
   reader.set(_valueTag,&value,false);
   reader.read();
   _value = value.text().toInt(&_hasValue);
}
