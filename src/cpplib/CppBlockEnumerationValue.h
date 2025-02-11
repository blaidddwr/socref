#ifndef CPP_BLOCK_ENUMERATIONVALUE_H
#define CPP_BLOCK_ENUMERATIONVALUE_H
#include "CppBlockBase.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ enumeration value.
 * 
 * assignment: This property holds any assignment for this enumeration value. If
 * this value has no assignment this property is an empty string.
 */
class EnumerationValue:
    public Base
{
    Q_OBJECT
    QString _assignment;


    /*!
     * Notify method.
     */
    signals:
    void assignmentChanged(
        const QString& value
    );


    public:
    EnumerationValue(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    /*!
     * Getter method.
     */
    public:
    const QString& assignment(
    ) const;


    public:
    virtual AbstractBlockWidget* createWidget(
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
     * Setter method.
     */
    public:
    void setAssignment(
        const QString& value
    );


    public:
    virtual void setState(
        const QHash<QString,QVariant>& state
    ) override final;


    public:
    virtual QHash<QString,QVariant> state(
    ) const override final;


    protected:
    virtual AbstractBlock* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
