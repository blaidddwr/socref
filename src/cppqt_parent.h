#ifndef CPPQT_PARENT_H
#define CPPQT_PARENT_H
#include "basic_block.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the parent block. This represents a single inheritance class for
    * another C++ class. This contains the class name being inherited and its
    * access type of public, protected, or private.
    */
   class Parent : public Basic::Block
   {
      Q_OBJECT
   public:
      virtual QIcon icon() const override final;
   public:
      /*!
       * Constructs a new parent block with a default state or null state based off
       * the given flag.
       */
      Q_INVOKABLE explicit Parent() = default;
   public:
      QString accessString() const;
      QString className() const;
   private:
      /*!
       * Defines all possible access types a parent class can have.
       */
      enum Access
      {
         /*!
          * Defines the public type.
          */
         Public
         /*!
          * Defines the protected type.
          */
         ,Protected
         /*!
          * Defines the private type.
          */
         ,Private
      };
   private:
      /*!
       * List of access type names that follow the same order as this block's
       * enumeration of possible access types.
       */
      static const QStringList _accessNames;
   };
}

#endif
