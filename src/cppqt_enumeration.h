#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_base.h"
#include "cppqt.h"



namespace CppQt
{
   class Enumeration : public Base
   {
      Q_OBJECT
   public:
      explicit Enumeration() = default;
      explicit Enumeration(const QString& name);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() override final;
      bool isClass() const;
      void setClass(bool isClass);
      QList<EnumValue*> values() const;
   protected:
      virtual void readData(const QDomElement& data, int version) override final;
      virtual int writeVersion() const override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      void readVersion0(const QDomElement& data);
      void readVersion1(const QDomElement& data);
      constexpr static int _verison {1};
      static const char* _classTag;
      bool _class {false};
   };
}



#endif
