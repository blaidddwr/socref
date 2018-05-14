#include "cppqt_variable.h"
#include <exception.h>
#include "cppqt_view_variable.h"
#include "cppqt_edit_variable.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_type.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
const char* Variable::_constExprTag {"constexpr"};
const char* Variable::_staticTag {"static"};
const char* Variable::_mutableTag {"mutable"};
const char* Variable::_typeTag {"type"};
const char* Variable::_initializerTag {"initializer"};






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
   QString ret {Base::name()};
   if ( !_initializer.isEmpty() )
   {
      if ( isFunctionArgument() ) ret.append(" =");
      else ret.append(" {}");
   }
   ret.append(attributes());
   return ret;
}






int Variable::type() const
{
   return BlockFactory::VariableType;
}






QIcon Variable::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon static_;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/variable.svg");
      static_ = QIcon(":/icons/svariable.svg");
   }
   if ( _static ) return static_;
   else return regular;
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
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
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
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






bool Variable::isMutable() const
{
   return _mutable;
}






void Variable::setMutable(bool isMutable)
{
   if ( isMutable && !isClassMember() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set as mutable when it is not a class member."));
      throw e;
   }
   if ( _mutable != isMutable )
   {
      _mutable = isMutable;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
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
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






bool Variable::hasInitializer() const
{
   return !_initializer.isEmpty();
}






QString Variable::initializer() const
{
   return _initializer;
}






void Variable::setInitializer(const QString& initializer)
{
   if ( _initializer != initializer )
   {
      _initializer = initializer;
      notifyModified();
      notifyNameModified();
      notifyBodyModified();
   }
}






bool Variable::isClassMember() const
{
   if ( parent()->type() == BlockFactory::AccessType ) return true;
   else return false;
}






bool Variable::isFunctionArgument() const
{
   return qobject_cast<Function*>(parent());
}






void Variable::readData(const QDomElement& data, int version)
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






int Variable::writeVersion() const
{
   return _version;
}






QDomElement Variable::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   if ( _constExpr ) ret.appendChild(document.createElement(_constExprTag));
   if ( _static ) ret.appendChild(document.createElement(_staticTag));
   if ( _mutable ) ret.appendChild(document.createElement(_mutableTag));
   ret.appendChild(makeElement(document,_typeTag,_type));
   if ( !_initializer.isEmpty() )
   {
      ret.appendChild(makeElement(document,_initializerTag,_initializer));
   }
   return ret;
}






unique_ptr<AbstractBlock> Variable::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Variable);
}






void Variable::copyDataFrom(const AbstractBlock* object)
{
   if ( const Variable* object_ = qobject_cast<const Variable*>(object) )
   {
      Base::copyDataFrom(object);
      _type = object_->_type;
      _constExpr = object_->_constExpr;
      _static = object_->_static;
      _mutable = object_->_mutable;
      _initializer = object_->_initializer;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






QString Variable::attributes() const
{
   QString ret;
   if ( _constExpr ) ret.append("X");
   if ( _mutable ) ret.append("M");
   if ( !ret.isEmpty() ) ret.prepend(" ");
   return ret;
}






void Variable::checkTypeSyntax(const QString& type)
{
   if ( !Type::isValidTypeString(type) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
      throw e;
   }
}






void Variable::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _constExpr = reader.attributeToInt(_constExprTag,false);
   _static = reader.attributeToInt(_staticTag,false);
   _type = reader.attribute(_typeTag);
   _initializer = reader.attribute(_initializerTag,false);
}






void Variable::readVersion1(const QDomElement& data)
{
   DomElementReader reader(data);
   reader.set(_constExprTag,&_constExpr,false);
   reader.set(_staticTag,&_static,false);
   reader.set(_mutableTag,&_mutable,false);
   reader.set(_typeTag,&_type);
   reader.set(_initializerTag,&_initializer,false);
   reader.read();
   if ( !reader.allRequiredFound() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed reading all required elements."));
      throw e;
   }
}
