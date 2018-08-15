#ifndef CPPQT_SIGNAL_H
#define CPPQT_SIGNAL_H
#include "cppqt_slot.h"
//



namespace CppQt
{
   /*!
    * This is the signal block. This represents signals for the Qt signaling system. 
    * This inherits from the base function class since signals are basically 
    * specialized methods. Signals do not have any return type, properties, or steps 
    * of operation. Because there is no steps of operation the edit dialog for this 
    * block type does not use the specialized function edit dialog. 
    */
   class Signal : public Slot
   {
      Q_OBJECT
   public:
      class Edit;
   public:
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      explicit Signal(bool isDefault = false);
   protected:
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
