#ifndef BLOCK_CPP_VARIABLE_H
#define BLOCK_CPP_VARIABLE_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ variable.
 * 
 * Its properties are type and assignment. Both are self-explanatory.
 */
class Variable:
    public Block::Cpp::Base
{
    Q_OBJECT
    QString _assignment;
    QString _type {"int"};


    /*!
     * Signals this instance's assignment property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void assignmentChanged(
        const QString& value
    );


    /*!
     * Signals this instance's type property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void typeChanged(
        const QString& value
    );


    public:
    Variable(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual ~Variable(
    ) override;


    /*!
     * Getter method.
     *
     * @return
     * This instance's assignment property.
     */
    public:
    const QString& assignment(
    ) const;


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


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    /*!
     * Sets this instance's assignment property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setAssignment(
        const QString& value
    );


    /*!
     * Sets this instance's type property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setType(
        const QString& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's type property.
     */
    public:
    const QString& type(
    ) const;


    protected:
    virtual void addEvent(
        int index
    ) override final;


    protected:
    virtual void moveEvent(
        int from
        ,int to
    );


    protected:
    virtual void removeEvent(
        int index
    ) override final;
};
}
}


#endif
