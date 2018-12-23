#ifndef CPPQT_DECLARATION_H
#define CPPQT_DECLARATION_H
#include "basicblock.h"
//



namespace CppQt
{
   /*!
    * This is the declaration block. This represents declarations found in C++ 
    * classes. While declarations can be made outside of classes this block is limited 
    * to declarations made within class declarations. 
    */
   class Declaration : public BasicBlock
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new using block. 
       */
      Q_INVOKABLE explicit Declaration() = default;
      QString line() const;
   };
}



#endif
