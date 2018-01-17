#include "cppqt_type_modifiers.h"
#include "exception.h"



using namespace CppQt::Type;






QString Modifiers::fullNameWithModifers(const QString& name) const
{
   QString ret;
   switch (_base)
   {
   case Const:
      ret.append("const ");
      break;
   case ConstExpr:
      ret.append("constexpr ");
   }
   if ( _isStatic )
   {
      ret.append("static ");
   }
   ret.append(name);
   for (auto pointer : _pointers)
   {
      ret.append("*");
      if ( pointer == Const )
      {
         ret.append("const");
      }
   }
   if ( _isReference )
   {
      ret.append("&");
   }
   return ret;
}






Modifiers& Modifiers::setBase(int base)
{
   if ( base < 0 || base >= Total )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid base argument of %1 for C++/Qt modifers.").arg(base));
      throw e;
   }
   _base = base;
   return *this;
}






Modifiers& Modifiers::setStatic(bool isStatic)
{
   _isStatic = isStatic;
   return *this;
}






Modifiers& Modifiers::setReference(bool isReference)
{
   _isReference = isReference;
   return *this;
}






Modifiers& Modifiers::setPointers(const QList<int>& pointers)
{
   for (auto pointer : pointers)
   {
      if ( pointer < 0 || pointer >= ConstExpr )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Invalid pointer type of %1 in list for C++/Qt modifers.").arg(pointer));
         throw e;
      }
   }
   _pointers = pointers;
   return *this;
}






QString Modifiers::toString() const
{
   QStringList ret;
   ret.append(QString::number(_base));
   ret.append(QString::number(_isStatic));
   ret.append(QString::number(_isReference));
   for (auto pointer : _pointers)
   {
      ret.append(QString::number(pointer));
   }
   return ret.join(":");
}






Modifiers& Modifiers::fromString(const QString& text)
{
   auto fail = [&text,this]()
   {
      _base = NotConst;
      _isStatic = false;
      _pointers.clear();
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid string '%1' to set as C++/Qt modifiers object.").arg(text));
      throw e;
   };
   QStringList data {text.split(":")};
   if ( data.size() < 3 )
   {
      fail();
   }
   bool ok;
   _base = data.at(0).toInt(&ok);
   if ( !ok )
   {
      fail();
   }
   _isStatic = data.at(1).toInt(&ok);
   if ( !ok )
   {
      fail();
   }
   _isReference = data.at(2).toInt(&ok);
   if ( !ok )
   {
      fail();
   }
   for (int i = 3; i < data.size() ;++i)
   {
      _pointers.append(data.at(i).toInt(&ok));
      if ( !ok )
      {
         fail();
      }
   }
   return *this;
}
