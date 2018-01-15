#include "cppqt_type_modifiers.h"
#include "exception.h"



using namespace CppQt::Type;






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
   for (auto pointer : _pointers)
   {
      ret.append(QString::number(pointer));
   }
   return ret.join(":");
}






Modifiers& Modifiers::fromString(const QString& text)
{
   auto fail = [&text](Modifiers* this_)
   {
      this_->_base = NotConst;
      this_->_isStatic = false;
      this_->_pointers.clear();
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid string '%1' to set as C++/Qt modifiers object.").arg(text));
      throw e;
   };
   QStringList data {text.split(":")};
   if ( data.size() < 2 )
   {
      fail(this);
   }
   bool ok;
   _base = data.at(0).toInt(&ok);
   if ( !ok )
   {
      fail(this);
   }
   _isStatic = data.at(1).toInt(&ok);
   if ( !ok )
   {
      fail(this);
   }
   for (int i = 2; i < data.size() ;++i)
   {
      _pointers.append(data.at(i).toInt(&ok));
      if ( !ok )
      {
         fail(this);
      }
   }
   return *this;
}
