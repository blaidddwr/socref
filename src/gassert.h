#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <iostream>
#define G_ASSERT(cond) if (!(cond)) { std::cout << "Assertion Failed @ " << __FILE__ << ":" << __LINE__ << "\n"; exit(1); }
#endif
