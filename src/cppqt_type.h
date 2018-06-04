#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include <abstractblock.h>
//



namespace CppQt
{
   /*!
    * This is the type block. This is used to define a specific C++ type, either 
    * literal or template. It also provides helper syntax validation static functions 
    * which other classes within the C++/Qt project type can use. 
    */
   class Type : public AbstractBlock
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
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
       * Constructs a new type block. 
       */
      explicit Type() = default;
      explicit Type(const QString& type);
   protected:
      /*!
       * Defines the fields this block contains. 
       */
      enum Field
      {
         /*!
          * Defines the type name field. This field is used to store the actual C++ type 
          * declaration. 
          */
         TypeName
         /*!
          * Defines the total number of fields this block defines. 
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
