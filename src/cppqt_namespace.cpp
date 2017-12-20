#include <memory>

#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






void Namespace::readData(QXmlStreamReader& xml)
{
   enum
   {
      Name = 0
      ,Description
   };
   XMLElementParser parser(xml);
   parser.addKeyword("name",true).addKeyword("description",true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Name:
         _name = xml.readElementText();
         break;
      case Description:
         _description = xml.readElementText();
         break;
      }
   }
   if ( !parser.allRead() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading in all required elements."));
      throw e;
   }
}






void Namespace::writeData(QXmlStreamWriter& xml) const
{
   xml.writeTextElement("name",_name);
   xml.writeTextElement("description",_description);
   if ( xml.hasError() )
   {
      Exception::WriteError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Xml Error writing to file."));
      throw e;
   }
}






AbstractBlock* Namespace::makeCopy() const
{
   unique_ptr<Namespace> ret {new Namespace(factory(),type())};
   ret->_name = _name;
   ret->_description = _description;
   ret->copyChildren(this);
   return ret.release();
}






void Namespace::setName(const QString& name)
{
   if ( _name != name )
   {
      _name = name;
      notifyOfNameChange();
      emit modified();
   }
}






void CppQt::Namespace::setDescription(const QString& description)
{
   if ( _description != description )
   {
      _description = description;
      emit modified();
   }
}






QList<QString> Namespace::variableTypes() const
{
   QList<QString> ret;
   if ( !parent() )
   {
      ret << "bool" << "char" << "short int" << "int" << "long int" << "long long int"
          << "short unsigned int" << "unsigned int" << "long unsigned int"
          << "long long unsigned int" << "float" << "double" << "qint8" << "qint16" << "qint32"
          << "qint64" << "quint8" << "quint16" << "quint32" << "quint64";
   }
   for (int i = 0; i < childrenSize() ;++i)
   {
      if ( child(i)->type() == BlockFactory::NamespaceType )
      {
         const Namespace* child_ {qobject_cast<const Namespace*>(child(i))};
         QList<QString> childList {child_->variableTypes()};
         for (auto& x : childList)
         {
            x.prepend(child_->name() + "::");
         }
         ret << childList;
      }
   }
   return ret;
}
