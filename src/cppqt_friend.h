#ifndef CPPQT_FRIEND_H
#define CPPQT_FRIEND_H
#include "cppqt_using.h"
//



namespace CppQt
{
   /*!
    * This is the friend block. This represents C++ friend declarations used in 
    * classes. Because this is almost identical to using declarations this block 
    * inherits almost everything from that block. The only difference is that the 
    * declaration begins with friend instead of using. 
    */
   class Friend : public Using
   {
      Q_OBJECT
   public:
      explicit Friend(bool isDefault = false);
      virtual int type() const override final;
      virtual QString name() const override final;
   protected:
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
      virtual void checkField(const QString& value);
   };
}



#endif
