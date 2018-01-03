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
      virtual void readData(QXmlStreamReader& xml) override final;
      virtual void writeData(QXmlStreamWriter& xml) const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      void setName(const QString& name);
      QString description() const { return _description; }
      void setDescription(const QString& description);
   private:
      virtual QString scopeName() const override final { return _name; }
      QString _name;
      QString _description;
   };
}



#endif
