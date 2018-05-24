#ifndef CPPQT_SLOT_H
#define CPPQT_SLOT_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    */
   class Slot : public Function
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
      explicit Slot() = default;
      explicit Slot(const QString& name);
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
