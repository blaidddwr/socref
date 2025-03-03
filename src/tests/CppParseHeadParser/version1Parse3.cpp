#ifndef TEST_H
#define TEST_H
#include <one>
#include \"two.h\"
#define THREE 0
using header1;
using ok = header2::ok;

class Foo
{
};

void Bar();

//nsfooter:
class Simple {};
class AnotherOne : public Simple { using Simple::Simple; }


/*!
 * comments
 */

#endif
