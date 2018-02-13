#ifndef CPPQT_BLOCKFACTORY_H
#define CPPQT_BLOCKFACTORY_H
#include "abstractblockfactory.h"
#include "singleton.h"



namespace CppQt
{
   class BlockFactory : public AbstractBlockFactory, public Singleton<BlockFactory>
   {
   public:
      enum
      {
         NamespaceType
         ,VariableType
         ,FunctionType
         ,TemplateType
         ,ClassType
         ,AccessType
         ,OperatorType
         ,SlotType
         ,SignalType
         ,ConstructorType
         ,DestructorType
         ,EnumerationType
         ,EnumValueType
         ,Total
      };
      virtual int size() const override final;
      virtual QString name(int type) const override final;
      virtual QString elementName(int type) const override final;
      virtual std::unique_ptr<AbstractBlock> makeRootBlock() const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const override final;
      virtual int type() const override final;
   };
}



#endif
