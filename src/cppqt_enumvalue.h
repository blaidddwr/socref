#ifndef CPPQT_ENUMVALUE_H
#define CPPQT_ENUMVALUE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the enumeration value block. This defines a single value for a C++ 
    * enumeration. This also allows for setting itself to a specific integer value. 
    * Because the integer can be another enumeration value or a macro this value is 
    * allowed to be a string. 
    */
   class EnumValue : public Base
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
   protected:
      virtual Sut::QPtr<BasicBlock::View> makeBasicView() const override final;
   };
}



#endif
