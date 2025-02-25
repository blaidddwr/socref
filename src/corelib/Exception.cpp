/*@ version 1 @*/
#include "Exception.h"
namespace Exception {

Base::Base(const QString& message):
    _message(message)
{
}

const QString& Base::message() const
{
    return _message;
}

}
