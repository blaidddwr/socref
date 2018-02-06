#ifndef CPPQT_NAMESPACE_H
#define CPPQT_NAMESPACE_H
#include "cppqt_base.h"



namespace CppQt
{
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      Namespace() = default;
      Namespace(const QString& name);
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      QStringList types() const;
      void setTypes(const QStringList& types);
      Namespace* root();
      const Namespace* root() const;
   signals:
      void typesChanged();
   protected:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      void copyDataFrom(const Namespace& object);
   private:
      void readType(const QDomElement& type);
      static const char* _typeTag;
      static const char* _nameTag;
      QStringList _types;
   };
}



#endif
