#include "cppqt_parent.h"
#include <exception.h>
#include "cppqt_view_parent.h"
#include "cppqt_edit_parent.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const QStringList Parent::_accessNames {"public","protected","private"};
const char* Parent::_accessTag {"access"};
const char* Parent::_templateArgumentTag {"template"};
const char* Parent::_nameTag {"name"};
const char* Parent::_classTag {"class"};






Parent::Parent(const QString& className):
   _class(className)
{}






QString Parent::name() const
{
   return accessName();
}






int Parent::type() const
{
   return BlockFactory::ParentType;
}






const AbstractBlockFactory&Parent::factory() const
{
   return BlockFactory::instance();
}






QIcon Parent::icon() const
{
   static bool isLoaded {false};
   static QIcon public_;
   static QIcon protected_;
   static QIcon private_;
   if ( !isLoaded )
   {
      public_ = QIcon(":/icons/parent.svg");
      protected_ = QIcon(":/icons/proparent.svg");
      private_ = QIcon(":/icons/priparent.svg");
      isLoaded = true;
   }
   switch (_access)
   {
   case Access::Public: return public_;
   case Access::Protected: return protected_;
   case Access::Private: return private_;
   default: return public_;
   }
}






QList<int> Parent::buildList() const
{
   return QList<int>();
}






std::unique_ptr<QWidget> Parent::makeView() const
{
   return unique_ptr<QWidget>(new View::Parent(this));
}






std::unique_ptr<::Gui::AbstractEdit> Parent::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Parent(this));
}






const QStringList&Parent::accessNames() const
{
   return _accessNames;
}






QString Parent::accessName() const
{
   return _accessNames.at(static_cast<int>(_access));
}






Parent::Access Parent::access() const
{
   return _access;
}






void Parent::setAccess(Parent::Access access)
{
   if ( _access != access )
   {
      _access = access;
      notifyOfNameChange();
      emit modified();
   }
}






void Parent::setAccess(const QString& accessName)
{
   setAccess(static_cast<Access>(_accessNames.indexOf(accessName)));
}






QString Parent::className() const
{
   return _class;
}






void Parent::setClassName(const QString& className)
{
   if ( !Type::isValidTypeString(className) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Class name '%1' is not valid.").arg(className));
      throw e;
   }
   if ( _class != className )
   {
      _class = className;
      emit modified();
   }
}






void Parent::readData(const QDomElement& data, int version)
{
   switch (version)
   {
   case 0:
      readVersion0(data);
      break;
   case 1:
      readVersion1(data);
      break;
   case 2:
      readVersion2(data);
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






int Parent::writeVersion() const
{
   return _version;
}






QDomElement Parent::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.appendChild(makeElement(document,_accessTag,_accessNames.at(static_cast<int>(_access))));
   ret.appendChild(makeElement(document,_classTag,_class));
   return ret;
}






std::unique_ptr<AbstractBlock> Parent::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Parent);
}






void Parent::copyDataFrom(const AbstractBlock* object)
{
   if ( const Parent* object_ = qobject_cast<const Parent*>(object) )
   {
      _access = object_->_access;
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






void Parent::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _access = static_cast<Access>(_accessNames.indexOf(reader.attribute(_accessTag)));
   QString templateArgument {reader.attribute(_templateArgumentTag,false)};
   _class = reader.attribute(_nameTag,false);
   if ( !templateArgument.isEmpty() ) _class.append("<").append(templateArgument).append(">");
}






void Parent::readVersion1(const QDomElement& data)
{
   QString access;
   QString templateArgument;
   DomElementReader reader(data);
   reader.set(_accessTag,&access);
   reader.set(_templateArgumentTag,&templateArgument,false);
   reader.set(_nameTag,&_class,false);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
   _access = static_cast<Access>(_accessNames.indexOf(access));
   if ( !templateArgument.isEmpty() ) _class.append("<").append(templateArgument).append(">");
}






void Parent::readVersion2(const QDomElement& data)
{
   QString access;
   DomElementReader reader(data);
   reader.set(_accessTag,&access);
   reader.set(_classTag,&_class);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
   _access = static_cast<Access>(_accessNames.indexOf(access));
}
