#include "cppqt_type.h"
#include <QRegularExpression>
#include "cppqt_view_type.h"
#include "cppqt_edit_type.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Type::_typeTag {"type"};






bool Type::isValidTypeString(const QString& input)
{
   bool ret {false};
   if ( QRegExp("\\s*(const\\s+)?((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*(<(.*)>)?)+(\\s*\\*(\\s*const)?)*\\s*&{0,2}\\s*").exactMatch(input) )
   {
      ret = true;
      QRegularExpression regExp("<(.*)>");
      QRegularExpressionMatchIterator matches {regExp.globalMatch(input)};
      while ( matches.hasNext() )
      {
         QRegularExpressionMatch match {matches.next()};
         if ( !isValidTemplateArgument(match.captured(1)) )
         {
            ret = false;
            break;
         }
      }
   }
   return ret;
}






bool Type::isValidTemplateArgument(const QString& argument)
{
   bool ret {true};
   QStringList arguments {argument.split(',')};
   for (auto arg : arguments)
   {
      if ( QRegExp("\\s*((((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?)|([0-9]+(\\.[0-9]+)?))\\s*").exactMatch(arg) )
      {
         if ( arg.contains("<") )
         {
            int begin {arg.indexOf('<')};
            int end {arg.lastIndexOf('>')};
            if ( !isValidTemplateArgument(arg.mid(begin+1,end-begin-1)) )
            {
               ret = false;
               break;
            }
         }
      }
      else
      {
         ret = false;
         break;
      }
   }
   return ret;
}






Type::Type(const QString& type):
   _type(type)
{
   if ( !isValidTypeString(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given string '%1' is not a valid C++ type.").arg(type));
      throw e;
   }
}






int Type::type() const
{
   return BlockFactory::TypeType;
}






const AbstractBlockFactory& Type::factory() const
{
   return BlockFactory::instance();
}






QString Type::name() const
{
   return _type;
}






QIcon Type::icon() const
{
   QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/type.svg");
   return ret;
}






QList<int> Type::buildList() const
{
   return QList<int>();
}






std::unique_ptr<QWidget> Type::makeView() const
{
   return unique_ptr<QWidget>(new View::Type(this));
}






std::unique_ptr<::Gui::AbstractEdit> Type::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Type(this));
}






QString Type::cppType()
{
   return _type;
}






void Type::setType(const QString& type)
{
   if ( !isValidTypeString(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given string '%1' is not a valid C++ type.").arg(type));
      throw e;
   }
   if ( _type != type )
   {
      _type = type;
      emit modified();
      notifyOfNameChange();
   }
}






void Type::readData(const QDomElement& element, int version)
{
   Q_UNUSED(version)
   DomElementReader reader(element);
   reader.set(_typeTag,&_type);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
}






int Type::writeVersion() const
{
   return _version;
}






QDomElement Type::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.appendChild(makeElement(document,_typeTag,_type));
   return ret;
}






std::unique_ptr<AbstractBlock> Type::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Type);
}






void Type::copyDataFrom(const AbstractBlock* other)
{
   if ( const Type* valid = qobject_cast<const Type*>(other) )
   {
      _type = valid->_type;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
