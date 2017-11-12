#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "abstractblock.h"



namespace CppQt
{
   class Namespace : public AbstractBlock
   {
      Q_OBJECT
   public:
      explicit Namespace(const AbstractBlockFactory& factory, int type):
         AbstractBlock(factory,type),
         _name(tr("unnamed_namespace"))
         {}
      virtual QString getName() const override final { return _name; }
      virtual void readData(QXmlStreamReader& xml) override final;
      virtual void writeData(QXmlStreamWriter& xml) const override final;
      virtual AbstractBlock* makeCopy() const override final;
      void setName(const QString& name);
      QString getDescription() const { return _description; }
      void setDescription(const QString& description);
      QList<QString> getVariableTypes() const;
   private:
      QString _name;
      QString _description;
   };
}



#endif
