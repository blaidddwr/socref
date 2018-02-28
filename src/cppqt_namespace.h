#ifndef CPPQT_NAMESPACE_H
#define CPPQT_NAMESPACE_H
#include "cppqt_base.h"



namespace CppQt
{
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      explicit Namespace() = default;
      explicit Namespace(const QString& name);
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      virtual QList<AbstractBlock*> realChildren() const;
      QStringList types() const;
      void setTypes(const QStringList& types);
      Namespace* root();
      const Namespace* root() const;
   signals:
      void typesChanged();
   protected:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void copyDataFrom(const AbstractBlock* object) override;
   private:
      void readType(const QDomElement& type);
      static const char* _typeTag;
      static const char* _nameTag;
      QStringList _types;
   };
}



#endif
