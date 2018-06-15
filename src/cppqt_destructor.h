#ifndef CPPQT_DESTRUCTOR_H
#define CPPQT_DESTRUCTOR_H
#include "cppqt_constructor.h"
//



namespace CppQt
{
   /*!
    */
   class Destructor : public Constructor
   {
      Q_OBJECT
   public:
      class Edit;
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      /*!
       */
      explicit Destructor() = default;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
