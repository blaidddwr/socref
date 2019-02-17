#ifndef CPPQT_ACCESS_H
#define CPPQT_ACCESS_H
#include "basic_block.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the access block. This represents access types for C++ classes such
    * as public or private. This also defines Qt access types for slots and
    * signals. Almost all block types used with classes must be attached to an
    * access block instead of direct attachment because of how C++ works. All
    * supported types are public, protected, private, signals, public slots,
    * protected slots, and private slots.
    */
   class Access : public Basic::Block
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
   public:
      /*!
       * Constructs a new access block.
       */
      Q_INVOKABLE explicit Access() = default;
      bool isPrivate() const;
      bool isSignal() const;
      bool isSlot() const;
      bool hasVirtual() const;
      bool hasAbstract() const;
      QString accessString() const;
   protected:
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
      virtual bool childIsUpdated(Abstract::Block* child) override final;
      virtual bool childAdded(Abstract::Block* child) override final;
      virtual bool childRemoved(Abstract::Block* child) override final;
   private:
      /*!
       * Defines all possible access types.
       */
      enum Type
      {
         /*!
          * Defines the public type.
          */
         Public
         /*!
          * Defines the protected type.
          */
         ,Protected
         /*!
          * Defines the private type.
          */
         ,Private
         /*!
          * Defines the signals type.
          */
         ,Signals
         /*!
          * Defines the public slots type.
          */
         ,PublicSlots
         /*!
          * Defines the protected slots type.
          */
         ,ProtectedSlots
         /*!
          * Defines the private slots type.
          */
         ,PrivateSlots
      };
   private:
      /*!
       * List of access type names that follow the same order as this block's
       * enumeration of possible types.
       */
      static const QStringList _typeNames;
   };
}

#endif
