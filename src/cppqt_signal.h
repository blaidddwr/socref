#ifndef CPPQT_SIGNAL_H
#define CPPQT_SIGNAL_H
#include "cppqt_slot.h"



namespace CppQt
{
   class Signal : public Slot
   {
      Q_OBJECT
   public:
      explicit Signal() = default;
      explicit Signal(const QString& name);
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
