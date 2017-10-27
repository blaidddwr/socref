#ifndef CPPQT_BLOCKFACTORY_H
#define CPPQT_BLOCKFACTORY_H
#include "abstractblockfactory.h"



namespace CppQt {

class BlockFactory : public AbstractBlockFactory
{
public:
   enum
   {
      NamespaceType = 0
      ,Total
   };
   BlockFactory(int type);
   virtual int getSize() const override final;
   virtual QString getName(int type) const override final;
   virtual QIcon getIcon(int type) const override final;
   virtual const QList<int> getBuildList(int type) const override final;
   virtual AbstractBlock* makeRootBlock() const override final;
   virtual AbstractBlock* makeBlock(int type) const override final;
   virtual Gui::AbstractView* makeView(int type) const override final;
   virtual Gui::AbstractEdit* makeEdit(int type) const override final;
};



//@@
inline BlockFactory::BlockFactory(int type): AbstractBlockFactory(type) {}

//@@
inline int BlockFactory::getSize() const { return Total; }

} // namespace CppQt



#endif
