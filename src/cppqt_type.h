#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include <abstractblock.h>
//



namespace CppQt
{
   /*!
    */
   class Type : public AbstractBlock
   {
      Q_OBJECT
   public:
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      static bool isValidTypeString(const QString& value);
      static bool isValidTemplateArgument(const QString& value);
      /*!
       */
      explicit Type() = default;
      explicit Type(const QString& type);
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         TypeName
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
   private:
      static void checkType(const QString& value);
      void setType(const QString& value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      QString _type;
   };
}



#endif
