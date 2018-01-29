#include "cppqt_variable.h"
#include "cppqt_gui_typedialog.h"
#include "exception.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace CppQt;



const char* Variable::_typeTag {"type"};






Variable::Variable(const QString& name):
   Base(name)
{}






Variable::Variable(const QString& type, const QString& name):
   Base(name),
   _type(type)
{
   checkTypeSyntax(type);
}






QString Variable::name() const
{
   return Base::name().prepend(" ").prepend(_type);
}






std::unique_ptr<AbstractBlock> Variable::makeCopy() const
{
   unique_ptr<Variable> ret {new Variable};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_type = _type;
   return ret;
}






int Variable::type() const
{
   return BlockFactory::VariableType;
}






QString Variable::variableType()
{
   return _type;
}






void Variable::setVariableType(const QString& type)
{
   checkTypeSyntax(type);
   if ( _type != type )
   {
      _type = type;
      notifyOfNameChange();
      emit modified();
   }
}






void Variable::readData(const QDomElement& data)
{
   Base::readData(data);
   DomElementReader reader(data);
   _type = reader.attribute(_typeTag);
}






QDomElement Variable::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   ret.setAttribute(_typeTag,_type);
   return ret;
}






void Variable::checkTypeSyntax(const QString& type)
{
   if ( !QRegExp(Gui::TypeDialog::_typeRegExp).exactMatch(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
      throw e;
   }
}
