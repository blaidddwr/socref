#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "abstractblock.h"



//@@
namespace CppQt {

//@@
class Namespace : public AbstractBlock
{
   Q_OBJECT
public:
   explicit Namespace(const AbstractBlockFactory& factory, int type);
   virtual QString getName() const override final;
   virtual void readData(QXmlStreamReader& xml) override final;
   virtual void writeData(QXmlStreamWriter& xml) const override final;
   virtual AbstractBlock* makeCopy() const override final;
   void setName(const QString& name);
private:
   QString _name;
};



//@@
inline Namespace::Namespace(const AbstractBlockFactory &factory, int type):
   AbstractBlock(factory,type), _name(tr("unnamed_namespace")) {}

//@@
inline QString Namespace::getName() const { return _name; }

//@@
inline AbstractBlock* Namespace::makeCopy() const { return new Namespace(getFactory(),getType()); }

}



#endif
