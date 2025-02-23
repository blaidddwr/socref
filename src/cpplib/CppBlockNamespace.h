#ifndef CPP_BLOCK_NAMESPACE_H
#define CPP_BLOCK_NAMESPACE_H
#include "AbstractBlock.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ namespace.
 * 
 * It can be expanded for other blocks, providing protected methods to interact
 * with its properties.
 * 
 * @property name The name.
 * 
 * @property description The description.
 */
class Namespace:
    public AbstractBlock
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
    Namespace(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual AbstractBlockWidget* createWidget(
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
    virtual QIcon displayIcon(
    ) const override;


    public:
    virtual QString displayText(
    ) const override;


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
    Namespace(
        const QString& name
        ,Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    protected:
    virtual AbstractBlock* create(
        QObject* parent = nullptr
    ) const override;


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
     * The key used for this block's description property.
     */
    protected:
    static const char* descriptionKey(
    );


    /*!
     * Getter method.
     *
     * @return
     * The key used for this block's name property.
     */
    protected:
    static const char* nameKey(
    );
};
}
}


#endif
