#ifndef BASICBLOCKFACTORY_H
#define BASICBLOCKFACTORY_H
#include <QDomElement>
#include "abstractblockfactory.h"
#include "global.h"
//



/*!
 */
class BasicBlockFactory : public AbstractBlockFactory
{
public:
   virtual int size() const override final;
   virtual QString name(int type) const override final;
   virtual int typeByElementName(const QString& elementName) const override final;
   virtual QString elementName(int type) const override final;
   virtual Sut::QPtr<AbstractBlock> makeBlock(int type, bool isDefault) const override final;
   virtual Sut::QPtr<AbstractBlock> makeRootBlock() const override final;
public:
   BasicBlockFactory(const QString& xmlPath, const QStringList& elementNames);
   virtual ~BasicBlockFactory() override;
protected:
   /*!
    *
    * @param index  
    */
   virtual Sut::QPtr<BasicBlock> makeBasicBlock(int index) = 0;
private:
   /*!
    */
   QVector<QString> _typeNames;
   /*!
    */
   QStringList _typeElementNames;
   /*!
    */
   QVector<QDomElement*> _typeDefinitions;
   /*!
    */
   int _rootType {-1};
};



#endif
