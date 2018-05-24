#ifndef CPPQT_SIGNAL_H
#define CPPQT_SIGNAL_H
#include "cppqt_slot.h"
//



namespace CppQt
{
   /*!
    */
   class Signal : public Slot
   {
      Q_OBJECT
   public:
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      /*!
       */
      explicit Signal() = default;
      explicit Signal(const QString& name);
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
