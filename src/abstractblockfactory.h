#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <QString>
#include <QIcon>



class AbstractBlock;

namespace Gui {

class AbstractView;
class AbstractEdit;

} // namespace Gui



//@@
class AbstractBlockFactory
{
public:
   AbstractBlockFactory(int type);
   //@@
   virtual int getSize() const = 0;
   //@@
   virtual QString getName(int type) const = 0;
   //@@
   virtual QString getElementName(int type) const = 0;
   //@@
   virtual QIcon getIcon(int type) const = 0;
   //@@
   virtual const QList<int> getBuildList(int type) const = 0;
   //@@
   virtual AbstractBlock* makeRootBlock() const = 0;
   //@@
   virtual AbstractBlock* makeBlock(int type) const = 0;
   //@@
   virtual QWidget* makeView(int type, AbstractBlock* block) const = 0;
   //@@
   virtual Gui::AbstractEdit* makeEdit(int type) const = 0;
   int getType() const;
private:
   int _type;
};



//@@
inline AbstractBlockFactory::AbstractBlockFactory(int type): _type(type) {}

//@@
inline int AbstractBlockFactory::getType() const { return _type; }



#endif
