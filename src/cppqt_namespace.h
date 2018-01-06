#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_abstractcontainer.h"



namespace CppQt
{
   class Namespace : public AbstractContainer
   {
      Q_OBJECT
   public:
      virtual QString name() const override final { return _name; }
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      Namespace& setName(const QString& name);
      QString description() const { return _description; }
      Namespace& setDescription(const QString& description);
   private:
      virtual AbstractBlock* readData(QXmlStreamReader& xml) override final;
      virtual const AbstractBlock* writeData(QXmlStreamWriter& xml) const override final;
      virtual QString scopeName() const override final { return _name; }
      QString _name;
      QString _description;
   };
}



#endif
