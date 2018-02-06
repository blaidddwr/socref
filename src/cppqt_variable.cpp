#include "cppqt_variable.h"
#include "cppqt_view_variable.h"
#include "cppqt_edit_variable.h"
#include "cppqt_gui_typedialog.h"
#include "exception.h"
#include "cppqt_blockfactory.h"
#include "domelementreader.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






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






QIcon Variable::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/variable.svg");
   return ret;
}






QList<int> Variable::buildList() const
{
   return QList<int>();
}






unique_ptr<QWidget> Variable::makeView() const
{
   return unique_ptr<QWidget>(new View::Variable(this));
}






unique_ptr<AbstractEdit> Variable::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Variable(this));
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
   if ( parent()->type() == BlockFactory::AccessType ) return true;
   else if ( parent()->type() == BlockFactory::NamespaceType || parent()->type() == BlockFactory::FunctionType ) return false;
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Variable's parent is of an unexpected type '%1'.").arg(factory().name(parent()->type())));
      throw e;
   }
}






bool Variable::isFunctionArgument() const
{
   return parent()->type() == BlockFactory::FunctionType;
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
   if ( _constExpr ) ret.setAttribute(_constExprTag,_constExpr);
   if ( _static ) ret.setAttribute(_staticTag,_static);
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
   if ( _constExpr ) ret.append("constexpr ");
   if ( _static ) ret.append("static ");
   return ret;
}






void Variable::checkTypeSyntax(const QString& type)
{
   if ( !Gui::TypeDialog::isValidTypeString(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
      throw e;
   }
}
