#ifndef CPPQT_ENUMVALUE_H
#define CPPQT_ENUMVALUE_H
#include "basicblock.h"
//



namespace CppQt
{
   /*!
    * This is the enumeration value block. This defines a single value for a C++ 
    * enumeration. This also allows for setting itself to a specific integer value. 
    * Because the integer can be another enumeration value or a macro this value is 
    * allowed to be a string. 
    */
   class EnumValue : public BasicBlock
   {
      Q_OBJECT
   public:
      class View;
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new enumeration value block. 
       */
      Q_INVOKABLE explicit EnumValue() = default;
      bool hasValue() const;
      QString value() const;
   };
}



#endif
