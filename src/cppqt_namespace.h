#ifndef CPPQT_NAMESPACE_H
#define CPPQT_NAMESPACE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the namespace block. This defines a C++ namespace. An instance of this 
    * block is used as the root block for C++/Qt projects as the global namespace. 
    */
   class Namespace : public Base
   {
      Q_OBJECT
   public:
      /*!
       * Constructs a new namespace block. 
       */
      Q_INVOKABLE explicit Namespace() = default;
      const Namespace* root() const;
      Namespace* root();
   };
}



#endif
