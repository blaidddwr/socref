#ifndef CPPQT_CONSTRUCTOR_H
#define CPPQT_CONSTRUCTOR_H
#include "cppqt_function.h"



namespace CppQt
{
   class Constructor : public Function
   {
      Q_OBJECT
   public:
      explicit Constructor() = default;
      virtual QString name() const override;
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      QString className() const;
      bool isExplicit() const;
      void setExplicit(bool isExplicit);
   public slots:
      void classNameChanged();
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      static const char* _explicitTag;
      bool _explicit {false};
   };
}



#endif
