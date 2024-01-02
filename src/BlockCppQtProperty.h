#ifndef BLOCK_CPPQT_PROPERTY_H
#define BLOCK_CPPQT_PROPERTY_H
#include "BlockCppProperty.h"
namespace Block {
namespace CppQt {




/*!
 * This is a C++/Qt block class. It represents a C++/Qt property.
 * 
 * It extends the C++ property with the three new properties. These properties
 * are used to generate the Qt property macro call in order to register it with
 * the Qt meta object property system.
 * 
 * Its properties are read method, write method, and notify signal. All these
 * methods are strings used to generate the Qt property macro line, registering
 * it with the Qt meta object system.
 */
class Property:
    public Block::Cpp::Property
{
    Q_OBJECT
    QString _notifySignal;
    QString _readMethod;
    QString _writeMethod;
    public:
    using Block::Cpp::Property::Property;


    /*!
     * Signals this instance's notify signal property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void notifySignalChanged(
        const QString& value
    );


    /*!
     * Signals this instance's read method property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void readMethodChanged(
        const QString& value
    );


    /*!
     * Signals this instance's write method property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void writeMethodChanged(
        const QString& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's notify signal property.
     */
    public:
    const QString& notifySignal(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's read method property.
     */
    public:
    const QString& readMethod(
    ) const;


    /*!
     * Sets this instance's notify signal property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setNotifySignal(
        const QString& value
    );


    /*!
     * Sets this instance's read method property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setReadMethod(
        const QString& value
    );


    /*!
     * Sets this instance's write method property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setWriteMethod(
        const QString& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's write method property.
     */
    public:
    const QString& writeMethod(
    ) const;
};
}
}


#endif
