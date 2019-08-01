#ifndef CPPQT_CLASS_H
#define CPPQT_CLASS_H
#include "cppqt_namespace.h"
#include "cppqt.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the class block. This represents C++ classes. This inherits from the
    * namespace block since a class expands upon the concept of a namespace.
    * Because many properties of a class depend on the blocks it contains the class
    * block is very dependent upon its child block properties.
    */
   class Class : public Namespace
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
   public:
      /*!
       * Constructs a new class block.
       */
      Q_INVOKABLE explicit Class() = default;
   public:
      bool isQtObject() const;
      bool isVirtual() const;
      bool isAbstract() const;
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
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
      virtual bool childIsUpdated(Abstract::Block* child) override final;
      virtual bool childAdded(Abstract::Block* child) override final;
      virtual bool childRemoved(Abstract::Block* child) override final;
   };
}

#endif
