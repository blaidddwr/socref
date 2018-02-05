#include "cppqt_variable.h"
#include "cppqt_gui_typedialog.h"
#include "exception.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace CppQt;



const char* Variable::_typeTag {"type"};
const char* Variable::_constExprTag {"constexpr"};
const char* Variable::_staticTag {"static"};






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
   return properties().append(_type).append(" ").append(Base::name());
}






std::unique_ptr<AbstractBlock> Variable::makeCopy() const
{
   unique_ptr<Variable> ret {new Variable};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   return ret;
}






int Variable::type() const
{
   return BlockFactory::VariableType;
}






QString Variable::elementName() const
{
   return QString("variable");
}






QIcon Variable::icon() const
{
   static QIcon ret;
   if ( ret.isNull() )
   {
      ret = QIcon(":/icons/variable.svg");
   }
   return ret;
}






QList<int> Variable::buildList() const
{
   return QList<int>();
}






bool Variable::isConstExpr() const
{
   return _constExpr;
}






void Variable::setConstExpr(bool isConstExpr)
{
   if ( isConstExpr && isFunctionArgument() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is a function argument."));
      throw e;
   }
   if ( _constExpr != isConstExpr )
   {
      _constExpr = isConstExpr;
      notifyOfNameChange();
      emit modified();
   }
}






bool Variable::isStatic() const
{
   return _static;
}






void Variable::setStatic(bool isStatic)
{
   if ( isStatic && !isClassMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as static when it is not a class member."));
      throw e;
   }
   if ( _static != isStatic )
   {
      _static = isStatic;
      notifyOfNameChange();
      emit modified();
   }
}






QString Variable::variableType() const
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






bool Variable::isClassMember() const
{
   const AbstractBlock* root {this};
   while ( root->parent() )
   {
      root = root->parent();
      if ( root->type() == BlockFactory::ClassType )
      {
         return true;
      }
      else if ( root->type() == BlockFactory::NamespaceType || root->type() == BlockFactory::FunctionType )
      {
         return false;
      }
   }
   Exception::LogicError e;
   MARK_EXCEPTION(e);
   e.setDetails(tr("Reached root of project without finding a single namespace or class."));
   throw e;
}






bool Variable::isFunctionArgument() const
{
   const AbstractBlock* root {this};
   while ( root->parent() )
   {
      root = root->parent();
      if ( root->type() == BlockFactory::FunctionType )
      {
         return true;
      }
   }
   return false;
}






void Variable::readData(const QDomElement& data)
{
   Base::readData(data);
   DomElementReader reader(data);
   _type = reader.attribute(_typeTag);
   _constExpr = reader.attributeToInt(_constExprTag,false);
   _static = reader.attributeToInt(_staticTag,false);
}






QDomElement Variable::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   ret.setAttribute(_typeTag,_type);
   ret.setAttribute(_constExprTag,_constExpr);
   ret.setAttribute(_staticTag,_static);
   return ret;
}






void Variable::copyDataFrom(const Variable& object)
{
   Base::copyDataFrom(object);
   _type = object._type;
   _constExpr = object._constExpr;
   _static = object._static;
}






QString Variable::properties() const
{
   QString ret;
   if ( _constExpr )
   {
      ret.append("constexpr ");
   }
   if ( _static )
   {
      ret.append("static ");
   }
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
