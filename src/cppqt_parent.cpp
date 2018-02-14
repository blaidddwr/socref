#include <exception.h>
#include "cppqt_parent.h"
#include "cppqt_view_parent.h"
#include "cppqt_edit_parent.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "cppqt_common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const QStringList Parent::_accessNames {"public","protected","private"};
const char* Parent::_accessTag {"access"};
const char* Parent::_templateArgumentTag {"template"};






Parent::Parent(const QString& name):
   Base(name)
{}






QString Parent::name() const
{
   QString ret {_accessNames.at(static_cast<int>(_access))};
   ret.append(" ").append(Base::name());
   if ( !_templateArgument.isEmpty() ) ret.append("<").append(_templateArgument).append(">");
   return ret;
}






int Parent::type() const
{
   return BlockFactory::ParentType;
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
      protected_ = QIcon(":/icons/parent.svg");
      private_ = QIcon(":/icons/parent.svg");
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






std::unique_ptr<Gui::AbstractEdit> Parent::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Parent(this));
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






QString Parent::templateArgument() const
{
   return _templateArgument;
}






void Parent::setTemplateArgument(const QString& templateArgument)
{
   if ( !templateArgument.isEmpty() && !isValidTemplateArgument(templateArgument) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Template argument '%1' is not valid.").arg(templateArgument));
      throw e;
   }
   if ( _templateArgument != templateArgument )
   {
      _templateArgument = templateArgument;
      notifyOfNameChange();
      emit modified();
   }
}






void Parent::readData(const QDomElement& data)
{
   Base::readData(data);
   DomElementReader reader(data);
   _access = static_cast<Access>(_accessNames.indexOf(reader.attribute(_accessTag)));
   _templateArgument = reader.attribute(_templateArgumentTag,false);
}






QDomElement Parent::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   ret.setAttribute(_accessTag,_accessNames.at(static_cast<int>(_access)));
   if ( !_templateArgument.isEmpty() ) ret.setAttribute(_templateArgumentTag,_templateArgument);
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
      Base::copyDataFrom(object);
      _access = object_->_access;
      _templateArgument = object_->_templateArgument;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
