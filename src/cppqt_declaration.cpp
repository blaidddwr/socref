#include "cppqt_declaration.h"
#include <exception.h>
#include "cppqt_view_declaration.h"
#include "cppqt_edit_declaration.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const QStringList Declaration::_typeNames {"using","friend"};
const char* Declaration::_typeTag {"type"};
const char* Declaration::_fieldTag {"field"};






bool Declaration::isValidUsingString(const QString& text)
{
   return QRegExp("((::)?[a-zA-Z_]+[a-zA-Z_0-9]*)+").exactMatch(text);
}






bool Declaration::isValidFriendString(const QString& text)
{
   return QRegExp("[a-zA-Z_]+[a-zA-Z_0-9<>\\*& ]*\\([a-zA-Z_0-9<>\\*&, ]*\\)").exactMatch(text);
}






bool Declaration::isValidDeclare(const QString& type, const QString& field)
{
   Type typeEnum {static_cast<Type>(_typeNames.indexOf(type))};
   if ( typeEnum == Type::Using && !isValidUsingString(field) ) return false;
   if ( typeEnum == Type::Friend && !isValidFriendString(field) ) return false;
   return true;
}






Declaration::Declaration(Declaration::Type type):
   _type(type)
{}






Declaration::Declaration(Declaration::Type type, const QString& field):
   _type(type),
   _field(field)
{}






int Declaration::type() const
{
   return BlockFactory::DeclarationType;
}






const AbstractBlockFactory&Declaration::factory() const
{
   return BlockFactory::instance();
}






QString Declaration::name() const
{
   return _typeNames.at(static_cast<int>(_type));
}






QIcon Declaration::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/declaration.svg");
   return ret;
}






QList<int> Declaration::buildList() const
{
   return QList<int>();
}






std::unique_ptr<QWidget> Declaration::makeView() const
{
   return unique_ptr<QWidget>(new View::Declaration(this));
}






std::unique_ptr<::Gui::AbstractEdit> Declaration::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Declaration(this));
}






const QStringList&Declaration::declareTypes() const
{
   return _typeNames;
}






QString Declaration::declareTypeName() const
{
   return _typeNames.at(static_cast<int>(_type));
}






Declaration::Type Declaration::declareType() const
{
   return _type;
}






void Declaration::setDeclareType(Declaration::Type type)
{
   if ( _type != type )
   {
      _type = type;
      notifyOfNameChange();
      emit modified();
   }
}






void Declaration::setDeclareType(const QString& type)
{
   setDeclareType(static_cast<Type>(_typeNames.indexOf(type)));
}






QString Declaration::field() const
{
   return _field;
}






void Declaration::setField(const QString& field)
{
   if ( _type == Type::Using && !isValidUsingString(field) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The field '%1' is not a valid using string.").arg(field));
      throw e;
   }
   if ( _type == Type::Friend && !isValidFriendString(field) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The field '%1' is not a valid friend string.").arg(field));
      throw e;
   }
   if ( _field != field )
   {
      _field = field;
      emit modified();
   }
}






void Declaration::readData(const QDomElement& element, int version)
{
   Q_UNUSED(version)
   DomElementReader reader(element);
   QString type;
   reader.set(_typeTag,&type);
   reader.set(_fieldTag,&_field,false);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements for block."));
      throw e;
   }
   _type = static_cast<Type>(_typeNames.indexOf(type));
}






int Declaration::writeVersion() const
{
   return _version;
}






QDomElement Declaration::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.appendChild(makeElement(document,_typeTag,_typeNames.at(static_cast<int>(_type))));
   if ( !_field.isEmpty() )
   {
      ret.appendChild(makeElement(document,_fieldTag,_field));
   }
   return ret;
}






std::unique_ptr<AbstractBlock> Declaration::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Declaration);
}






void Declaration::copyDataFrom(const AbstractBlock* other)
{
   if ( const Declaration* valid = qobject_cast<const Declaration*>(other) )
   {
      _type = valid->_type;
      _field = valid->_field;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
