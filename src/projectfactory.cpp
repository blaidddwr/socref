#include <QObject>

#include "projectfactory.h"

//TEMPORARY until actual block factories are defined!
#include "abstractblockfactory.h"
class DummyFactory : public AbstractBlockFactory
{
   virtual int getSize() const { return 0; }
   virtual QString getName() const { return QString(); }
   virtual QList<int> getBuildList(int) const { return QList<int>(); }
   virtual AbstractBlock* makeBlock(int) const { return nullptr; }
   virtual Gui::AbstractView* makeView(int) const { return nullptr; }
   virtual Gui::AbstractEdit* makeEdit(int) const { return nullptr; }
};
DummyFactory g_placeHolderDummyFactory;






QString ProjectFactory::getName(int type) const
{
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   case PhpType: return QObject::tr("PHP");
   default: return QString();
   }
}






QString ProjectFactory::getDefaultFilters(int type) const
{
   Q_UNUSED(type);
   return QString("not yet implemented.");
}






const AbstractBlockFactory& ProjectFactory::getBlockFactory(int type) const
{
   Q_UNUSED(type);
   return g_placeHolderDummyFactory;
}
