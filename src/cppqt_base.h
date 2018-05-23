#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    */
   class Base : public AbstractBlock
   {
      Q_OBJECT
   public:
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override;
      virtual int fieldSize() const override;
      virtual AbstractBlock::Field fieldType(int index) const override;
      virtual QVariant field(int index) const override;
   public:
      /*!
       */
      explicit Base() = default;
      explicit Base(const QString& name);
      QString description() const;
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         Name
         /*!
          */
         ,Description
         /*!
          */
         ,Total
      };
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual QStringList fields() const;
   private:
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      QString _name;
      /*!
       */
      QString _description;
   };
}



#endif
