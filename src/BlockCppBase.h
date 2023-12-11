#ifndef BLOCK_CPP_BASE_H
#define BLOCK_CPP_BASE_H
#include "BlockAbstract.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It partially implements the abstract block class.
 * It represents a base block for C++ with properties all C++ blocks share.
 * 
 * Its properties are name, description, and scope name. Name and description
 * are self-explanatory.
 * 
 * The scope name is used to construct a C++ block's scope. Each scope name of a
 * block and it's parents are combined with "::" as their separator. If a scope
 * name is a null string then it is ignored and not combined with the other
 * scope names. The root namespace block's name scope must be null.
 */
class Base:
    public Block::Abstract
{
    Q_OBJECT
    QString _description;
    QString _name;
    public:
    using Block::Abstract::Abstract;


    /*!
     * Signals this block's description property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void descriptionChanged(
        const QString& value
    );


    /*!
     * Signals this block's name property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void nameChanged(
        const QString& value
    );


    public:
    virtual QString displayText(
    ) const override;


    /*!
     * Getter method.
     *
     * @return
     * This block's description property.
     */
    public:
    const QString& description(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This block's name property.
     */
    public:
    const QString& name(
    ) const;


    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override;


    public:
    virtual QString scope(
    ) const override;


    /*!
     * Sets this block's description property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setDescription(
        const QString& value
    );


    /*!
     * Sets this block's name property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setName(
        const QString& value
    );


    /*!
     * Called when this block's name property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    protected:
    virtual void onNameChanged(
        const QString& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's scope name property.
     */
    protected:
    virtual QString scopeName(
    ) const;
};
}
}


#endif
