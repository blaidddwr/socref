#ifndef CPPQT_FACTORY_H
#define CPPQT_FACTORY_H
#include "basicblockfactory.h"
#include <socutil/sut_singleton.h>
//



namespace CppQt
{
   /*!
    * This is the block factory for the C++/Qt project type. This class defines an 
    * enumeration representing all possible block types for its project type. This is 
    * a singleton class and has only one single global instance. 
    */
   class Factory : public BasicBlockFactory, public Sut::Singleton<Factory>
   {
   public:
      /*!
       * Defines all possible block types for the C++/Qt project type. 
       */
      enum 
      {
         /*!
          * Defines the type list block. This is for storing variable/function types because 
          * C++ is a strongly typed language. 
          */
         TypeListType
         /*!
          * Defines the type block. This defines a specific type a variable or a function's 
          * return can make as their type. 
          */
         ,TypeType
         /*!
          * Defines the namespace block. 
          */
         ,NamespaceType
         /*!
          * Defines the enumeration block. This is for C++ enumerations of all types. 
          */
         ,EnumerationType
         /*!
          * Defines the enumeration value block. This is for specific enumeration values. 
          */
         ,EnumValueType
         /*!
          * Defines the variable block. 
          */
         ,VariableType
         /*!
          * Defines the function block. 
          */
         ,FunctionType
         /*!
          * Defines the template block. This is used for template arguments in classes and 
          * functions. 
          */
         ,TemplateType
         /*!
          * Defines the class block. 
          */
         ,ClassType
         /*!
          * Defines the access block. This is used for the different access types in classes 
          * such as public, private, signals, or slots. 
          */
         ,AccessType
         /*!
          * Defines the operator block. 
          */
         ,OperatorType
         /*!
          * Defines the slot type. This is for Qt slot functions. 
          */
         ,SlotType
         /*!
          * Defines the signal block. This is for Qt signal functions. 
          */
         ,SignalType
         /*!
          * Defines the constructor block. 
          */
         ,ConstructorType
         /*!
          * Defines the destructor block. 
          */
         ,DestructorType
         /*!
          * Defines the parent block. This is for adding parents to a class's inheritance. 
          */
         ,ParentType
         /*!
          * Defines the using block. This is for using declarations within classes. 
          */
         ,UsingType
         /*!
          * Defines the friend block. This is for friend declarations within classes. 
          */
         ,FriendType
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
      virtual Sut::QPtr<BasicBlock> makeBasicBlock(int type) const;
   };
}



#endif
