#ifndef CPPQT_CONSTRUCTOR_H
#define CPPQT_CONSTRUCTOR_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    * This is the constructor block. This represents constructors for C++ classes. 
    * This inherits from the base function block since this is a specialized function 
    * type. 
    */
   class Constructor : public Function
   {
      Q_OBJECT
   public:
      class Edit;
   public:
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      QString className() const;
   public slots:
      void classNameChanged();
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual bool checkName(const QString& value) override final;
   };
}



#endif
