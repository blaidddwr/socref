#ifndef BLOCK_CPP_BASE_H
#define BLOCK_CPP_BASE_H
#include "BlockAbstract.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It partially implements the abstract block class.
 * It represents a base block for C++ with properties all C++ blocks share.
 * 
 * Its properties are name and description. Name and description are
 * self-explanatory.
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


    public:
    virtual QString displayText(
    ) const override;


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
};
}
}


#endif
