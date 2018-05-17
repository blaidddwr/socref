#include "cppqt_class.h"
#include <exception.h>
#include "cppqt_view_class.h"
#include "cppqt_edit_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_template.h"
#include "cppqt_constructor.h"
#include "cppqt_parent.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Class::_qtObjectTag {"qtobject"};






Class::Class(const QString& name):
   Namespace(name)
{}






QString Class::name() const
{
   QString ret;
   if ( hasTemplates() ) ret.append("<> ");
   ret.append(Base::name());
   if ( containsType(BlockFactory::ParentType) ) ret.append(" :");
   return ret;
}






int Class::type() const
{
   return BlockFactory::ClassType;
}






QIcon Class::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/class.svg");
      virtual_ = QIcon(":/icons/vclass.svg");
      abstract = QIcon(":/icons/aclass.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






QList<int> Class::buildList() const
{
   QList<int> ret {BlockFactory::AccessType,BlockFactory::ParentType,BlockFactory::TypeListType};
   if ( !isVirtual() && !_qtObject ) ret << BlockFactory::TemplateType;
   return ret;
}






unique_ptr<QWidget> Class::makeView() const
{
   return unique_ptr<QWidget>(new View::Class(this));
}






unique_ptr<AbstractEdit> Class::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Class(this));
}






QList<AbstractBlock*> Class::realChildren() const
{
   QList<AbstractBlock*> ret;
   for (auto child : makeListOfType<Access>(BlockFactory::AccessType))
   {
      ret << child << child->list();
   }
   return ret;
}






bool Class::isQtObject() const
{
   return _qtObject;
}






void Class::setQtObject(bool isQtObject)
{
   if ( !isQtObject && hasSignalsOrSlots() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("class must be Qt Object because it has slots and/or signals."));
      throw e;
   }
   if ( _qtObject != isQtObject )
   {
      _qtObject = isQtObject;
      notifyModified();
      notifyBodyModified();
   }
}






bool Class::isVirtual() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasVirtual() ) return true;
   }
   return false;
}






bool Class::isAbstract() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasAbstract() ) return true;
   }
   return false;
}






bool Class::hasSignalsOrSlots() const
{
   for (auto access : accessChildren())
   {
      if ( access->hasSignalsOrSlots() ) return true;
   }
   return false;
}






bool Class::hasTemplates() const
{
   return containsType(BlockFactory::TemplateType);
}






bool Class::hasAnyTemplates() const
{
   if ( hasTemplates() ) return true;
   AbstractBlock* back {parent()};
   while ( back && back->type() != BlockFactory::NamespaceType )
   {
      if ( Class* valid = back->cast<Class>(BlockFactory::ClassType) )
      {
         if ( valid->hasTemplates() ) return true;
      }
      back = back->parent();
   }
   return false;
}






QList<Template*> Class::templates() const
{
   QList<Template*> ret;
   for (auto child : list())
   {
      if ( Template* valid = child->cast<Template>(BlockFactory::TemplateType) ) ret.append(valid);
   }
   return ret;
}






QList<Parent*> Class::parents() const
{
   return makeListOfType<Parent>(BlockFactory::ParentType);
}






void Class::readData(const QDomElement& data, int version)
{
   Namespace::readData(data,version);
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






int Class::writeVersion() const
{
   return _version;
}






QDomElement Class::writeData(QDomDocument& document) const
{
   QDomElement ret {Namespace::writeData(document)};
   if ( _qtObject ) ret.appendChild(document.createElement(_qtObjectTag));
   return ret;
}






unique_ptr<AbstractBlock> Class::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Class);
}






void Class::copyDataFrom(const AbstractBlock* object)
{
   if ( const Class* object_ = qobject_cast<const Class*>(object) )
   {
      Namespace::copyDataFrom(object);
      _qtObject = object_->_qtObject;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






bool Class::childNameModified(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   return false;
}






bool Class::childAdded(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      connect(this,&Class::nameChanged,constructor,&Constructor::classNameChanged);
   }
   return false;
}






bool Class::childRemoved(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child)
        || qobject_cast<Function*>(child)
        || qobject_cast<Parent*>(child) )
   {
      notifyNameModified();
   }
   if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      disconnect(constructor);
   }
   return false;
}






QList<Access*> Class::accessChildren() const
{
   QList<Access*> ret;
   for (auto child : list())
   {
      if ( Access* valid = child->cast<Access>(BlockFactory::AccessType) ) ret << valid;
   }
   return ret;
}






void Class::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _qtObject = reader.attributeToInt(_qtObjectTag,false);
}






void Class::readVersion1(const QDomElement& data)
{
   DomElementReader reader(data);
   reader.set(_qtObjectTag,&_qtObject,false);
   reader.read();
}
