#ifndef TEST_H
#define TEST_H
#include <one>
#include \"two.h\"
#define THREE 0
namespace Dummy {
using header1;
using ok = header2::ok;

class Foo
{
};

void Bar();

class Simple {};
class AnotherOne : public Simple { using Simple::Simple; }
}


/*!
 * comments
 */

#endif
