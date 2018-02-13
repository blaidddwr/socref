#ifndef CPPQT_ENUMVALUE_H
#define CPPQT_ENUMVALUE_H
#include "cppqt_base.h"



namespace CppQt
{
   class EnumValue : public Base
   {
      Q_OBJECT
   public:
      explicit EnumValue() = default;
      explicit EnumValue(const QString& name);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() override final;
      bool hasValue() const;
      int value() const;
      void clearValue();
      void setValue(int value);
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      static const char* _valueTag;
      bool _hasValue {false};
      int _value {0};
   };
}



#endif
