#ifndef BLOCK_CPP_CLASS_H
#define BLOCK_CPP_CLASS_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ class.
 * 
 * Its properties are parents and templates. The parents is a list of a class's
 * parent classes. The templates list is a list of template arguments for a
 * class.
 */
class Class:
    public Block::Cpp::Base
{
    Q_OBJECT
    QStringList _parents;
    QStringList _templates;
    public:
    using ::Block::Cpp::Base::Base;


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
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


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


    /*!
     * Getter method.
     *
     * @return
     * This block's templates property.
     */
    public:
    const QStringList& templates(
    ) const;
};
}
}


#endif
