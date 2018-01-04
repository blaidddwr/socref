#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_abstractcontainer.h"



namespace CppQt
{
   class Namespace : public AbstractContainer
   {
      Q_OBJECT
   public:
      explicit Namespace(const AbstractBlockFactory& factory, int type):
         AbstractContainer(factory,type),
         _name(tr("unnamed_namespace"))
         {}
      virtual QString name() const override final { return _name; }
      virtual AbstractBlock* readData(QXmlStreamReader& xml) override final;
      virtual const AbstractBlock* writeData(QXmlStreamWriter& xml) const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      Namespace& setName(const QString& name);
      QString description() const { return _description; }
      Namespace& setDescription(const QString& description);
   private:
      virtual QString scopeName() const override final { return _name; }
      QString _name;
      QString _description;
   };
}



#endif
