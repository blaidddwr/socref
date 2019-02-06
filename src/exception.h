#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>
//



/*!
 * This contains a simple message that can be used for throwing messages as an 
 * exception to communicate why something failed. 
 */
class Exception
{
public:
   Exception(const QString& message);
   QString message() const;
private:
   /*!
    * The message for this exception explaining what occurred. 
    */
   const QString _message;
};



#endif
