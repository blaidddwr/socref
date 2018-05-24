#ifndef CPPQT_OPERATOR_H
#define CPPQT_OPERATOR_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    */
   class Operator : public Function
   {
      Q_OBJECT
   public:
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual int fieldSize() const override;
      virtual AbstractBlock::Field fieldType(int index) const override;
      virtual QVariant field(int index) const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      /*!
       */
      explicit Operator() = default;
      explicit Operator(const QString& returnType);
      QString operation() const;
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         Operator
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual QStringList fields() const;
   private:
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      QString _operator;
   };
}



#endif
