#include "exception.h"



//






/*!
 * Constructs a new exception with the given error message. 
 *
 * @param message Error message explaining what occurred to cause this new 
 *                exception. 
 */
Exception::Exception(const QString& message):
   _message(message)
{}






/*!
 * Returns the message of this exception explaining the error that occurred. 
 *
 * @return The message of this exception. 
 */
QString Exception::message() const
{
   return _message;
}
