#ifndef CPPQT_SLOT_H
#define CPPQT_SLOT_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    * This is the slot block. This represents slots for the Qt signaling system. This 
    * inherits from the base function class since slots are basically specialized 
    * methods. Slots do not have any return type. 
    */
   class Slot : public Function
   {
      Q_OBJECT
   public:
      class Edit;
   public:
      virtual int type() const override;
      virtual QString name() const override final;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
   };
}



#endif
