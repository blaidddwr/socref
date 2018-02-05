#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "domelementreader.h"



using namespace std;
using namespace CppQt;






Class::Class(const QString& name):
   Namespace(name)
{}






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






QString Class::elementName() const
{
   return "class";
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
      ret << BlockFactory::VariableType;
      ret << BlockFactory::TemplateType;
   }
   return ret;
}






bool Class::isQtObject() const
{
   return _qtObject;
}






void Class::setQtObject(bool isQtObject)
{
   if ( !isQtObject && hasSlotsOrSignals() )
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
   //TODO requires AccessType first
}






bool Class::isAbstract() const
{
   //TODO requires AccessType first
}






bool Class::hasSlotsOrSignals() const
{
   //TODO requires AccessType first
}






void Class::readData(const QDomElement& data)
{
   Namespace::readData(data);
   DomElementReader reader(data);
   _qtObject = reader.attributeToInt("qtobject",false);
}






QDomElement Class::writeData(QDomDocument& document) const
{
   QDomElement ret {Namespace::writeData(document)};
   if ( _qtObject ) ret.setAttribute("qtobject",_qtObject);
   return ret;
}
