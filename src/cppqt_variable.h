#ifndef CPPQT_VARIABLE_H
#define CPPQT_VARIABLE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the variable block. This represents any global, class member, or 
    * function argument variable. This is also used as the base class for template 
    * arguments. This supports the constant expression, static, and mutable 
    * properties. The properties are checked to see if those properties make sense and 
    * an exception is thrown if one is set that is not valid. This also supports 
    * having an initial value. 
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override;
      virtual QString name() const override final;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override final;
      virtual Sut::QPtr<QWidget> makeView() const override;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override;
   public:
      explicit Variable(bool isDefault = false);
      bool isConstExpr() const;
      bool isStatic() const;
      bool isMutable() const;
      QString variableType() const;
      bool hasInitializer() const;
      QString initializer() const;
      bool isMember() const;
      bool isArgument() const;
   protected:
      /*!
       * Defines the fields this block contains in addition to its base fields. 
       */
      enum Field
      {
         /*!
          * Defines the constant expression field. This is a boolean property. Its value is 
          * set to extend the list of base fields this class inherits. 
          */
         ConstExpr = Base::Field::Total
         /*!
          * Defines the static field. This is a boolean property. 
          */
         ,Static
         /*!
          * Defines the mutable field. This is a boolean property. 
          */
         ,Mutable
         /*!
          * Defines the type field. This holds the C++ type for a variable block. 
          */
         ,Type
         /*!
          * Defines the initial value field. This holds any initial value a variable block 
          * may contain. 
          */
         ,Initializer
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual QStringList fields() const override final;
   protected:
      void setType(const QString& value);
   private:
      QString attributes() const;
      void setConstExpr(bool state);
      void setStatic(bool state);
      void setMutable(bool state);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. This is in addition to the base fields this block 
       * inherits. 
       */
      static const QStringList _fields;
      /*!
       * The constant expression property for this variable block. 
       */
      bool _constExpr {false};
      /*!
       * The static property for this variable block. 
       */
      bool _static {false};
      /*!
       * The mutable property for this variable block. 
       */
      bool _mutable {false};
      /*!
       * The C++ type for this variable block. 
       */
      QString _type;
      /*!
       * The initial value, if any, for this variable block. If there is no initial value 
       * then this is an empty string. 
       */
      QString _initializer;
   };
}



#endif
