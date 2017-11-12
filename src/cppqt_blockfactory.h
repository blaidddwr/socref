#ifndef CPPQT_BLOCKFACTORY_H
#define CPPQT_BLOCKFACTORY_H
#include "abstractblockfactory.h"



namespace CppQt
{
   class BlockFactory : public AbstractBlockFactory
   {
   public:
      enum
      {
         NamespaceType = 0
         ,Total
      };
      BlockFactory(int type): AbstractBlockFactory(type) {}
      virtual int getSize() const override final { return Total; }
      virtual QString getName(int type) const override final;
      virtual QString getElementName(int type) const override final;
      virtual QIcon getIcon(int type) const override final;
      virtual const QList<int> getBuildList(int type) const override final;
      virtual AbstractBlock* makeRootBlock() const override final;
      virtual AbstractBlock* makeBlock(int type) const override final;
      virtual QWidget* makeView(int type, AbstractBlock* block) const override final;
      virtual Gui::AbstractEdit* makeEdit(int type, AbstractBlock* block) const override final;
   };
}



#endif
