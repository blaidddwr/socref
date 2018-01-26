#include <memory>
#include <QDomDocument>
#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "cppqt_gui_typedialog.h"



using namespace std;
using namespace CppQt;






Namespace::Namespace(const QString& name):
   Base(name)
{}






unique_ptr<AbstractBlock> Namespace::makeCopy() const
{
   unique_ptr<Namespace> ret {new Namespace};
   ret->copyDataFrom(*this);
   ret->copyChildren(this);
   return ret;
}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






QStringList Namespace::types()
{
   return _types;
}






Namespace& Namespace::setTypes(const QStringList& types)
{
   QRegExp regexp(Gui::TypeDialog::_typeRegExp);
   for (auto type : types)
   {
      if ( !regexp.exactMatch(type) )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
         throw e;
      }
   }
   _types = types;
   emit modified();
   emit typesChanged();
   return *this;
}






Namespace* Namespace::root()
{
   Namespace* ret {qobject_cast<Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






void Namespace::readData(const QDomElement& data)
{
   Base::readData(data);
   enum
   {
      Type = 0
      ,Total
   };
   QStringList tags {"type"};
   QDomNode node {data.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (tags.indexOf(element.tagName()))
         {
         case Type:
            readType(element);
            break;
         }
      }
      node = node.nextSibling();
   }
}






QDomElement Namespace::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   for (auto typeName : qAsConst(_types))
   {
      QDomElement type {document.createElement("type")};
      type.setAttribute("name",typeName);
      ret.appendChild(type);
   }
   return ret;
}






void Namespace::readType(const QDomElement &type)
{
   if ( !type.hasAttribute("name") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element missing name attribute."));
      throw e;
   }
   if ( _types.contains(type.attribute("name")) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element found with duplicate entry."));
      throw e;
   }
   QString name {type.attribute("name")};
   _types.append(name);
}
