#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    */
   class Enumeration : public Base
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
      explicit Enumeration() = default;
      explicit Enumeration(const QString& name);
      bool isClass() const;
      QList<EnumValue*> values() const;
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         Class
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual QStringList fields() const override final;
   private:
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      bool _class {false};
   };
}



#endif
