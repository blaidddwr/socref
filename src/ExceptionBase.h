#ifndef EXCEPTION_BASE_H
#define EXCEPTION_BASE_H
#include <QString>
namespace Exception {




/*!
 * This is a base class. It it an exception representing an error. The type of
 * error is determined by the specific exception class which inherits it.
 * 
 * Its single property is message. The message provides a detailed description
 * of an exception's error.
 */
class Base
{
    QString _message;


    /*!
     * Constructs this new exception with the given message.
     *
     * @param message
     *        The message.
     */
    public:
    Base(
        const QString& message
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's message property.
     */
    public:
    const QString& message(
    ) const;
};
}


#endif
