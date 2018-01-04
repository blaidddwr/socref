#include "cppqt_type_template.h"
#include "exception.h"



using namespace std;
using namespace CppQt::Type;






Template::Template(const QString& name, QList<QString> variants):
   _name(name),
   _variants(variants)
{
   while ( _variants.size() > _values.size() )
   {
      _values.push_back(nullptr);
   }
}






bool Template::isConcrete() const
{
   for (auto value : _values)
   {
      if ( !value )
      {
         return false;
      }
      if ( !value->isConcrete() )
      {
         return false;
      }
   }
   return true;
}






bool Template::isEquivalent(const CppQt::AbstractType* type) const
{
   if ( const Template* type_ = dynamic_cast<const Template*>(type) )
   {
      return _name == type_->_name && _variants == type_->_variants;
   }
   return false;
}






QString Template::name(const QList<QString> scope) const
{
   QString ret {_name};
   if ( !_variants.isEmpty() )
   {
      ret.append("<");
      for (int i = 0; i < _variants.size() ;++i)
      {
         if ( _values.at(i) )
         {
            ret.append(_values.at(i)->name(scope));
         }
         else
         {
            ret.append(_variants.at(i));
         }
         if ( i != (_variants.size() - 1) )
         {
            ret.append(",");
         }
      }
      ret.append(">");
   }
   return ret;
}






Template& Template::setVariants(const QList<QString>& variants)
{
   _variants = variants;
   while ( _variants.size() > _values.size() )
   {
      _values.push_back(nullptr);
   }
   while ( _values.size() > _variants.size() )
   {
      delete _values.back();
      _values.pop_back();
   }
   return *this;
}






Template& Template::setValue(int index, unique_ptr<CppQt::AbstractType>&& type)
{
   if ( index >= _variants.size() )
   {
      ;//ERROR
   }
   delete _values.at(index);
   _values[index] = type.release();
   return *this;
}






Template& Template::clearValues()
{
   qDeleteAll(_values);
   for (auto& value : _values)
   {
      value = nullptr;
   }
   return *this;
}
