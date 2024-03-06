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
 */
class Base:
    public Abstract
{
    Q_OBJECT
    QString _description {"Detailed description."};
    QString _name;


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


    public:
    virtual QString fileName(
    ) const override;


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


    public:
    virtual void setState(
        const QHash<QString,QVariant>& state
    ) override;


    public:
    virtual QHash<QString,QVariant> state(
    ) const override;


    /*!
     * Constructs this new instance with the given name, meta, and parent. The
     * given meta must be valid and cannot be destroyed during the life of this
     * instance.
     *
     * @param name
     *        The name.
     *
     * @param meta
     *        The meta.
     *
     * @param parent
     *        The parent.
     */
    protected:
    Base(
        const QString& name
        ,Model::Meta::Block* meta
        ,QObject* parent = nullptr
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
