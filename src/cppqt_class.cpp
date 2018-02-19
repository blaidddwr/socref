#include "cppqt_class.h"
#include "cppqt_view_class.h"
#include "cppqt_edit_class.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "domelementreader.h"
#include "cppqt_access.h"
#include "cppqt_template.h"
#include "cppqt_common.h"
#include "cppqt_constructor.h"
#include "cppqt_parent.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Class::_qtObjectTag {"qtobject"};






Class::Class(const QString& name):
   Namespace(name)
{}






QString Class::name() const
{
   QString ret {templateName(this)};
   ret.append(Base::name());
   const QList<Parent*> list {makeChildListOfType<Parent>(BlockFactory::ParentType)};
   if ( !list.isEmpty() )
   {
      ret.append(" : ");
      bool first {true};
      for (auto parent : list)
      {
         if ( first ) first = false;
         else ret.append(", ");
         ret.append(parent->name());
      }
   }
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
   static QList<int> ret;
   if ( ret.isEmpty() )
   {
      ret << BlockFactory::TemplateType;
      ret << BlockFactory::AccessType;
      ret << BlockFactory::ParentType;
   }
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






bool Class::hasAnyTemplates() const
{
   if ( hasTemplates() ) return true;
   AbstractBlock* back {parent()};
   while ( back && back->type() != BlockFactory::NamespaceType )
   {
      if ( Class* back_ = qobject_cast<Class*>(back) )
      {
         if ( back_->hasTemplates() ) return true;
      }
   }
   return false;
}






QList<AbstractBlock*> Class::realChildren() const
{
   QList<AbstractBlock*> ret;
   const QList<Access*> accessList {makeChildListOfType<Access>(BlockFactory::AccessType)};
   for (auto child : accessList)
   {
      ret << child->children();
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
      notifyOfNameChange();
      emit modified();
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
   return hasChildOfType(BlockFactory::TemplateType);
}






QList<Template*> Class::templates() const
{
   QList<Template*> ret;
   const QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::TemplateType )
      {
         if ( Template* variable = qobject_cast<Template*>(child) ) ret.append(variable);
      }
   }
   return ret;
}






void Class::childNameChanged(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child) || qobject_cast<Function*>(child)  || qobject_cast<Parent*>(child) )
   {
      notifyOfNameChange();
      emit bodyChanged();
   }
}






void Class::childAdded(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child) || qobject_cast<Function*>(child) || qobject_cast<Parent*>(child) )
   {
      notifyOfNameChange();
      emit bodyChanged();
   }
   else if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      connect(this,&Class::nameChanged,constructor,&Constructor::classNameChanged);
   }
}






void Class::childRemoved(AbstractBlock* child)
{
   if ( qobject_cast<Template*>(child) || qobject_cast<Function*>(child) || qobject_cast<Parent*>(child) )
   {
      notifyOfNameChange();
      emit bodyChanged();
   }
   else if ( Constructor* constructor = qobject_cast<Constructor*>(child) )
   {
      disconnect(constructor);
   }
}






void Class::readData(const QDomElement& data)
{
   Namespace::readData(data);
   DomElementReader reader(data);
   _qtObject = reader.attributeToInt(_qtObjectTag,false);
}






QDomElement Class::writeData(QDomDocument& document) const
{
   QDomElement ret {Namespace::writeData(document)};
   if ( _qtObject ) ret.setAttribute(_qtObjectTag,_qtObject);
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






void Class::notifyOfNameChange()
{
   emit nameChanged();
   AbstractBlock::notifyOfNameChange();
}






QList<Access*> Class::accessChildren() const
{
   QList<Access*> ret;
   QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::AccessType )
      {
         if ( Access* access = qobject_cast<Access*>(child) ) ret << access;
      }
   }
   return ret;
}
