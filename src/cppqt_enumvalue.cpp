#include <exception.h>
#include "cppqt_enumvalue.h"
#include "cppqt_view_enumvalue.h"
#include "cppqt_edit_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



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






void EnumValue::readData(const QDomElement& data)
{
   Base::readData(data);
   if ( data.hasAttribute(_valueTag) )
   {
      _hasValue = true;
      DomElementReader reader(data);
      _value = reader.attributeToInt(_valueTag);
   }
   else _hasValue = false;
}






QDomElement EnumValue::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _hasValue ) ret.setAttribute(_valueTag,_value);
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
