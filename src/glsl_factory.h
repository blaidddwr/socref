#ifndef GLSL_FACTORY_H
#define GLSL_FACTORY_H
#include "basicblockfactory.h"
#include <socutil/sut_singleton.h>
//



namespace GLSL
{
   /*!
    * This is the block factory for the GLSL project type. This class defines an 
    * enumeration representing all possible block types for its project type. This is 
    * a singleton class and has only one single global instance. This also inherits 
    * from the basic block factory class to allow for XML definitions of block type 
    * data. 
    */
   class Factory : public BasicBlockFactory, public Sut::Singleton<Factory>
   {
   public:
      /*!
       * Defines all possible block types for the GLSL project type. 
       */
      enum 
      {
         /*!
          * Defines the namespace block. 
          */
         NamespaceType
         /*!
          * Defines the shader block. This is for an entire shader program and defines the 
          * main function defined in a shader program file. 
          */
         ,ShaderType
         /*!
          * Defines the variable block. 
          */
         ,VariableType
         /*!
          * Defines the struct block. This is used for structures that can be defined in 
          * shader programs. 
          */
         ,StructType
         /*!
          * Defines the function block. This is used to subroutine functions that can be 
          * defined in shader programs. 
          */
         ,FunctionType
         /*!
          * Defines the total number of unique blocks the GLSL project type contains. 
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
