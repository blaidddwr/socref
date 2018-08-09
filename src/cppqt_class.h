#ifndef CPPQT_CLASS_H
#define CPPQT_CLASS_H
#include "cppqt_namespace.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the class block. This represents C++ classes. This inherits from the 
    * namespace block since a class expands upon the concept of a namespace. Because 
    * many properties of a class depend on the blocks it contains the class block is 
    * very dependent upon its child blocks properties. 
    */
   class Class : public Namespace
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      virtual QList<AbstractBlock*> realChildren() const override final;
   public:
      explicit Class(bool isDefault = false);
      bool isQtObject() const;
      bool isVirtual() const;
      bool isAbstract() const;
      bool hasSignalsOrSlots() const;
      bool hasTemplates() const;
      bool hasAnyTemplates() const;
      QList<Template*> templates() const;
      QList<Parent*> parents() const;
   signals:
      /*!
       * Signals that the base name of this class has changed. This is used for the 
       * constructors of classes to know when they need to update their own name. 
       */
      void nameChanged();
   protected:
      /*!
       * Defines the fields this block contains in addition to its base fields. 
       */
      enum Field
      {
         /*!
          * Defines the qt object field. This is a boolean property. Its value is set to 
          * extend the list of base fields this class inherits. 
          */
         QtObject = Base::Field::Total
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      virtual QStringList fields() const override final;
   private:
      void setQtObject(bool state);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. This is in addition to the base fields this block 
       * inherits. 
       */
      static const QStringList _fields;
      /*!
       * The qt object property of this class block. 
       */
      bool _qtObject {false};
   };
}



#endif
