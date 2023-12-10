#ifndef BLOCK_TEST_NODE_H
#define BLOCK_TEST_NODE_H
#include "BlockAbstract.h"
namespace Block {
namespace Test {




/*!
 * This is a test block class. It represents no real language component or
 * object, instead being used by this application's test system.
 * 
 * Its single property is name. The name is self-explanatory.
 */
class Node:
    public Block::Abstract
{
    Q_OBJECT
    QString _name;
    public:
    using Block::Abstract::Abstract;


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    public:
    virtual QString displayText(
    ) const override final;


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
    ) override final;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    public:
    virtual QString scope(
    ) const override final;


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
};
}
}


#endif
