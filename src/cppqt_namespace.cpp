#include "cppqt_namespace.h"
#include <memory>
#include <QDomDocument>
#include <exception.h>
#include "cppqt_view_namespace.h"
#include "cppqt_edit_namespace.h"
#include "cppqt_gui_typedialog.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Namespace::_typeTag {"type"};
const char* Namespace::_nameTag {"name"};






Namespace::Namespace(const QString& name):
   Base(name)
{}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






QIcon Namespace::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/namespace.svg");
   return ret;
}






QList<int> Namespace::buildList() const
{
   return QList<int>
   {
      BlockFactory::NamespaceType
      ,BlockFactory::VariableType
      ,BlockFactory::FunctionType
      ,BlockFactory::ClassType
      ,BlockFactory::OperatorType
      ,BlockFactory::EnumerationType
   };
}






unique_ptr<QWidget> Namespace::makeView() const
{
   return unique_ptr<QWidget>(new View::Namespace(this));
}






unique_ptr<AbstractEdit> Namespace::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Namespace(this));
}






QList<AbstractBlock*> Namespace::realChildren() const
{
   return list();
}






QStringList Namespace::types() const
{
   return _types;
}






void Namespace::setTypes(const QStringList& types)
{
   for (auto type : types)
   {
      if ( !Gui::TypeDialog::isValidTypeString(type) )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
         throw e;
      }
   }
   if ( _types != types )
   {
      _types = types;
      emit modified();
      emit typesChanged();
   }
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






const Namespace* Namespace::root() const
{
   const Namespace* ret {qobject_cast<const Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






void Namespace::readData(const QDomElement& data, int version)
{
   Base::readData(data,version);
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






int Namespace::writeVersion() const
{
   return _version;
}






QDomElement Namespace::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   for (auto typeName : qAsConst(_types)) ret.appendChild(makeElement(document,_typeTag,typeName));
   return ret;
}






unique_ptr<AbstractBlock> Namespace::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}






void Namespace::copyDataFrom(const AbstractBlock* object)
{
   if ( const Namespace* object_ = qobject_cast<const Namespace*>(object) )
   {
      Base::copyDataFrom(object);
      _types = object_->_types;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






void Namespace::readVersion0(const QDomElement& data)
{
   QList<QDomElement> types;
   DomElementReader reader(data);
   reader.set(_typeTag,&types,false);
   reader.read();
   for (auto type : qAsConst(types)) readType(type);
}






void Namespace::readType(const QDomElement& type)
{
   DomElementReader reader(type);
   QString name {reader.attribute(_nameTag)};
   if ( _types.contains(name) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element found with duplicate entry."));
      throw e;
   }
   _types.append(name);
}






void Namespace::readVersion1(const QDomElement& data)
{
   QList<QDomElement> types;
   DomElementReader reader(data);
   reader.set(_typeTag,&types,false);
   reader.read();
   for (auto type : qAsConst(types)) _types.append(type.text());
}
