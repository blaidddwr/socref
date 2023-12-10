#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>
#define ASSERT(cond) if (!(cond)) { std::cout << "Assertion Failed @ " << __FILE__ << ":" << __LINE__ << "\n"; exit(1); }


/*!
 * This contains all exception classes and namespaces.
 * 
 * All specific exceptions inherit the base exception class.
 */
namespace Exception
{
    class Base;
}


#endif
