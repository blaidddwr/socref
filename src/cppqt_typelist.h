#ifndef CPPQT_TYPELIST_H
#define CPPQT_TYPELIST_H
#include "basicblock.h"
//



namespace CppQt
{
   /*!
    * This is the type list block. This is used to contain C++ defined types and other 
    * type lists to allow for nesting. This container block type is provided to allow 
    * users to better organize all defined types for a C++/Qt project because it is a 
    * hardly typed language and the list of defined types quickly becomes very long. 
    */
   class TypeList : public BasicBlock
   {
      Q_OBJECT
   public:
      class View;
   public:
      /*!
       * Constructs a new type list block. 
       */
      explicit TypeList() = default;
   protected:
      virtual Sut::QPtr<BasicBlock::View> makeBasicView() const override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
   };
}



#endif
