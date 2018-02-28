#include "cppqt_constructor.h"
#include <exception.h>
#include "cppqt_view_constructor.h"
#include "cppqt_edit_constructor.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Constructor::_explicitTag {"explicit"};






QString Constructor::name() const
{
   QString ret;
   if ( _explicit ) ret.append("explicit ");
   return ret.append(fullName(QString(),className()));
}






int Constructor::type() const
{
   return BlockFactory::ConstructorType;
}






QIcon Constructor::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/constructor.svg");
   return ret;
}






QList<int> Constructor::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






std::unique_ptr<QWidget> Constructor::makeView() const
{
   return unique_ptr<QWidget>(new View::Constructor(this));
}






std::unique_ptr<AbstractEdit> Constructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Constructor(this));
}






QString Constructor::className() const
{
   AbstractBlock* access {parent()};
   if ( !access || !access->parent() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent or grandparent is nullptr."));
      throw e;
   }
   if ( Base* base = qobject_cast<Base*>(access->parent()) )
   {
      return base->Base::name();
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Grandparent does not contain CppQt::Base class."));
      throw e;
   }
}






bool Constructor::isExplicit() const
{
   return _explicit;
}






void Constructor::setExplicit(bool isExplicit)
{
   if ( _explicit != isExplicit )
   {
      _explicit = isExplicit;
      notifyOfNameChange();
      emit modified();
   }
}






void Constructor::classNameChanged()
{
   notifyOfNameChange();
}






void Constructor::readData(const QDomElement& data)
{
   Function::readData(data);
   DomElementReader reader(data);
   _explicit = reader.attributeToInt(_explicitTag,false);
}






QDomElement Constructor::writeData(QDomDocument& document) const
{
   QDomElement ret {Function::writeData(document)};
   if ( _explicit ) ret.setAttribute(_explicitTag,_explicit);
   return ret;
}






std::unique_ptr<AbstractBlock> Constructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Constructor);
}






void Constructor::copyDataFrom(const AbstractBlock* object)
{
   if ( const Constructor* object_ = qobject_cast<const Constructor*>(object) )
   {
      Function::copyDataFrom(object);
      _explicit = object_->_explicit;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}
