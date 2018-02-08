#include "cppqt_class.h"
#include "cppqt_view_class.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "domelementreader.h"
#include "cppqt_access.h"
#include "cppqt_template.h"
#include "cppqt_common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Class::_qtObjectTag {"qtobject"};






Class::Class(const QString& name):
   Namespace(name)
{}






QString Class::name() const
{
   return templateName(this).append(Base::name());
}






unique_ptr<AbstractBlock> Class::makeCopy() const
{
   unique_ptr<Class> ret {new Class};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_qtObject = _qtObject;
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
   }
   return ret;
}






unique_ptr<QWidget> Class::makeView() const
{
   return unique_ptr<QWidget>(new View::Class(this));
}






unique_ptr<AbstractEdit> Class::makeEdit()
{
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
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Class::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
}






void Class::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyOfNameChange();
   emit bodyChanged();
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
