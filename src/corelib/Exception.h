/*@ version 1 @*/
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>
namespace Exception {

class Base
{
public:
    Base(const QString& message);
    const QString& message() const;
/*@ footer @*/
private:
    QString _message;
};

/*@ footer @*/
class FileSystem: public Base { using Base::Base; };
class LogicalBlock: public Base { using Base::Base; };
class LogicalParse: public Base { using Base::Base; };
class LogicalProject: public Base { using Base::Base; };
class LogicalRoute: public Base { using Base::Base; };
class ReadBlock: public Base { using Base::Base; };
class ReadProject: public Base { using Base::Base; };
class RunSystem: public Base { using Base::Base; };
class WriteBlock: public Base { using Base::Base; };
class WriteProject: public Base { using Base::Base; };
}

/*!
 * @namespace Exception
 * This contains all exceptions.
 *
 * All specific exceptions inherit the base exception class.
 */

/*!
 * @class Exception::Base
 * This is a base class. It it an exception representing an error. The type of
 * error is determined by the specific exception class which inherits it.
 *
 * Its single property is message. The message provides a detailed description
 * of an exception's error.
 */

/*!
 * @fn Exception::Base::Base
 * Constructs this new exception with the given message.
 *
 * @param message
 *        The message.
 */

/*!
 * @fn Exception::Base::message
 * Getter method.
 *
 * @return
 * This instance's message property.
 */

#endif
