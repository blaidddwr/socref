#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_base.h"



namespace CppQt
{
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      Namespace() = default;
      Namespace(const QString& name);
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      QStringList types();
      Namespace& setTypes(const QStringList& types);
      Namespace* root();
   signals:
      void typesChanged();
   protected:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
   private:
      void readType(const QDomElement& type);
      QStringList _types;
   };
}



#endif
