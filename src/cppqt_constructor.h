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
   public slots:
      void classNameChanged();
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
