#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "abstractblock.h"



namespace CppQt
{
   class Base : public AbstractBlock
   {
      Q_OBJECT
   public:
      Base() = default;
      Base(const QString& name);
      virtual QString name() const override;
      virtual const AbstractBlockFactory& factory() const override final;
      void setName(const QString& name);
      QString description() const;
      void setDescription(const QString& description);
   protected:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      void copyDataFrom(const Base& object);
   private:
      QString _name;
      QString _description;
   };
}



#endif
