#ifndef BLOCK_CPP_ENUMERATION_H
#define BLOCK_CPP_ENUMERATION_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ enumeration.
 * 
 * Its single property is class. It is self-explanatory.
 */
class Enumeration:
    public Block::Cpp::Base
{
    Q_OBJECT
    bool _class {false};


    public:
    Enumeration(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    /*!
     * Signals this block's class property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void classChanged(
        bool value
    );


    public:
    virtual Widget::Block::Abstract* createWidget(
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
     * This block's class property.
     */
    public:
    bool isClass(
    ) const;


    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override final;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    /*!
     * Sets this block's class property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setClass(
        bool value
    );


    public:
    virtual void setState(
        const QHash<QString,QVariant>& state
    ) override final;


    public:
    virtual QHash<QString,QVariant> state(
    ) const override final;


    protected:
    virtual Block::Abstract* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
