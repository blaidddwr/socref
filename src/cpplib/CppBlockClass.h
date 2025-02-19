#ifndef CPP_BLOCK_CLASS_H
#define CPP_BLOCK_CLASS_H
#include "CppBlockProperty.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ class.
 * 
 * Its properties are parents and templates. The parents is a list of a class's
 * parent classes. The templates list is a list of template arguments for a
 * class.
 */
class Class:
    public Property
{
    Q_OBJECT
    QStringList _parents;
    QStringList _templates;
    static bool _iconsInitialized;
    static const QIcon* _abstractIcon;
    static const QIcon* _regularIcon;
    static const QIcon* _virtualIcon;


    /*!
     * Signals this block's parents property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void parentsChanged(
        const QStringList& value
    );


    /*!
     * Signals this block's templates property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void templatesChanged(
        const QStringList& value
    );


    public:
    Class(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual AbstractBlockWidget* createWidget(
    ) const override final;


    public:
    virtual QString displayText(
    ) const override final;


    /*!
     * Initializes the icons used for the display icon property. This must be
     * called before creating any object of this class.
     */
    public:
    static void initializeIcons(
    );


    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override final;


    /*!
     * Getter method.
     *
     * @return
     * This block's parents property.
     */
    public:
    const QStringList& parents(
    ) const;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    /*!
     * Sets this block's parents property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setParents(
        const QStringList& value
    );


    public:
    virtual void setState(
        const QHash<QString,QVariant>& state
    ) override final;


    /*!
     * Set this block's templates property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setTemplates(
        const QStringList& value
    );


    public:
    virtual QHash<QString,QVariant> state(
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * This block's templates property.
     */
    public:
    const QStringList& templates(
    ) const;


    /*!
     * Forces this instance to update its display icon property.
     */
    public:
    virtual void updateDisplayIcon(
    ) override final;


    protected:
    virtual AbstractBlock* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
