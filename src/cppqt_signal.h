#ifndef CPPQT_SIGNAL_H
#define CPPQT_SIGNAL_H
#include "cppqt_base.h"



namespace CppQt
{
   class Signal : public Base
   {
      Q_OBJECT
   public:
      Signal() = default;
      Signal(const QString& name);
   };
}



#endif
