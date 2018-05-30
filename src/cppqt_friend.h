#ifndef CPPQT_FRIEND_H
#define CPPQT_FRIEND_H
#include "cppqt_using.h"
//



namespace CppQt
{
   /*!
    */
   class Friend : public Using
   {
      Q_OBJECT
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void checkField(const QString& value);
   };
}



#endif
