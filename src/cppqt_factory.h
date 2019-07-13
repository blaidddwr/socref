#ifndef CPPQT_FACTORY_H
#define CPPQT_FACTORY_H
#include "basic_blockfactory.h"
#include <socutil/soc_ut_singleton.h>



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the block factory for the C++/Qt project type. This class defines an
    * enumeration representing all possible block types for its project type. This
    * is a singleton class and has only one single global instance. This also
    * inherits from the basic block factory class to allow for XML definitions of
    * block type data.
    */
   class Factory : public Basic::BlockFactory, public Soc::Ut::Singleton<Factory>
   {
   public:
      /*!
       * Defines all possible block types for the C++/Qt project type.
       */
      enum Type
      {
         /*!
          * Defines the namespace block.
          */
         Namespace
         /*!
          * Defines the enumeration block. This is for C++ enumerations of all types.
          */
         ,Enumeration
         /*!
          * Defines the enumeration value block. This is for specific enumeration values.
          */
         ,EnumValue
         /*!
          * Defines the variable block.
          */
         ,Variable
         /*!
          * Defines the template block. This is used for template arguments in classes
          * and functions.
          */
         ,Template
         /*!
          * Defines the function block.
          */
         ,Function
         /*!
          * Defines the structure block.
          */
         ,Struct
         /*!
          * Defines the union block.
          */
         ,Union
         /*!
          * Defines the class block.
          */
         ,Class
         /*!
          * Defines the access block. This is used for the different access types in
          * classes such as public, private, signals, or slots.
          */
         ,Access
         /*!
          * Defines the parent block. This is for adding parents to a class's
          * inheritance.
          */
         ,Parent
         /*!
          * Defines the declaration block. This is for declarations within classes.
          */
         ,Declaration
         /*!
          * Defines the total number of unique blocks the C++/Qt project type contains.
          */
         ,Total
      };
   public:
      virtual int type() const override final;
      virtual QString elementName(int type) const override final;
   public:
      Factory();
   protected:
      virtual Soc::Ut::QPtr<Basic::Block> createBasicBlock(int type) const override final;
   };
}

#endif
