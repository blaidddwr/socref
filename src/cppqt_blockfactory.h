#ifndef CPPQT_BLOCKFACTORY_H
#define CPPQT_BLOCKFACTORY_H
#include "abstractblockfactory.h"
#include "singleton.h"
//



namespace CppQt
{
   /*!
    * This implements the abstract block factory that produces blocks for the C++/Qt 
    * project type. This class defines an enumeration representing all possible block 
    * types for its project type. This is a singleton class and has only one single 
    * global instance. 
    */
   class BlockFactory : public AbstractBlockFactory, public Singleton<BlockFactory>
   {
   public:
      /*!
       */
      enum 
      {
         /*!
          */
         NamespaceType
         /*!
          */
         ,VariableType
         /*!
          */
         ,FunctionType
         /*!
          */
         ,TemplateType
         /*!
          */
         ,ClassType
         /*!
          */
         ,AccessType
         /*!
          */
         ,OperatorType
         /*!
          */
         ,SlotType
         /*!
          */
         ,SignalType
         /*!
          */
         ,ConstructorType
         /*!
          */
         ,DestructorType
         /*!
          */
         ,EnumerationType
         /*!
          */
         ,EnumValueType
         /*!
          */
         ,ParentType
         /*!
          */
         ,DeclarationType
         /*!
          */
         ,TypeListType
         /*!
          */
         ,TypeType
         /*!
          */
         ,Total
      };
      virtual int type() const override final;
      virtual int size() const override final;
      virtual QString name(int type) const override final;
      virtual QString elementName(int type) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const override final;
      virtual std::unique_ptr<AbstractBlock> makeRootBlock() const override final;
   };
}



#endif
