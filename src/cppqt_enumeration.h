#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_base.h"
#include "cppqt.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the enumeration block. This is a container block which contains all
    * enumeration values for a C++ enumeration. This provides for unnamed, regular,
    * and class enumerations.
    */
   class Enumeration : public Base
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new enumeration block.
       */
      Q_INVOKABLE explicit Enumeration() = default;
   public:
      bool isClass() const;
      QList<EnumValue*> values() const;
   };
}

#endif
