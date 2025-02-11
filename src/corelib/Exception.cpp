/*@ version 1 @*/
#include "Exception.h"
namespace Exception {

/*@ method Base Base(const QString&) @*/
Base::Base(const QString& message):
    _message(message)
{
}

/*@ method Base message() @*/
const QString& Base::message() const
{
    return _message;
}

}
