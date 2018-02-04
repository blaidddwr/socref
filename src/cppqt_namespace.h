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
      virtual QString elementName() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      QStringList types();
      void setTypes(const QStringList& types);
      Namespace* root();
   signals:
      void typesChanged();
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
   private:
      void readType(const QDomElement& type);
      static const char* _typeTag;
      static const char* _nameTag;
      QStringList _types;
   };
}



#endif
