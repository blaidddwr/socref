#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <iostream>
#include "ExceptionBase.h"
#define G_ASSERT(cond) if (!(cond)) { std::cout << "Assertion Failed @ " << __FILE__ << ":" << __LINE__ << "\n"; exit(1); }


namespace Exception
{
namespace Block
{
class Logical: public Base { using Base::Base; };
class Read: public Base { using Base::Base; };
class Write: public Base { using Base::Base; };
}
namespace Project
{
class Logical: public Base { using Base::Base; };
class Read: public Base { using Base::Base; };
class Write: public Base { using Base::Base; };
}
namespace System
{
class File: public Base { using Base::Base; };
class Run: public Base { using Base::Base; };
}
}


#endif
