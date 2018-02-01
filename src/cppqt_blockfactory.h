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
         NamespaceType = 0
         ,VariableType
         ,FunctionType
         ,TemplateType
         ,Total
      };
      virtual int size() const override final;
      virtual QString name(int type) const override final;
      virtual QString elementName(int type) const override final;
      virtual QIcon icon(int type) const override final;
      virtual QList<int> buildList(int type) const override final;
      virtual std::unique_ptr<AbstractBlock> makeRootBlock() const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const override final;
      virtual std::unique_ptr<QWidget> makeView(int type, AbstractBlock* block) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit(int type, AbstractBlock* block) const override final;
      virtual int type() const override final;
   };
}



#endif
