#ifndef BLOCK_CPP_CLASS_H
#define BLOCK_CPP_CLASS_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ class.
 * 
 * Its properties are parents and template string. The parents is a list of a
 * class's parent classes. The template string is self-explanatory.
 */
class Class:
    public Block::Cpp::Base
{
    Q_OBJECT
    QStringList _parents;
    QString _template;
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
     * Signals this block's template string property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void templateStringChanged(
        const QString& value
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
     * Set this block's template string property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setTemplateString(
        const QString& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This block's template string property.
     */
    public:
    const QString& templateString(
    ) const;
};
}
}


#endif
