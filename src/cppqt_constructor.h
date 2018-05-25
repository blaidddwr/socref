#ifndef CPPQT_CONSTRUCTOR_H
#define CPPQT_CONSTRUCTOR_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    */
   class Constructor : public Function
   {
      Q_OBJECT
   public:
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      /*!
       */
      explicit Constructor() = default;
      QString className() const;
   public slots:
      void classNameChanged();
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
