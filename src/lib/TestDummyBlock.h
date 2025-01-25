#ifndef TEST_DUMMY_BLOCK_H
#define TEST_DUMMY_BLOCK_H
#include <QtGui>
#include "BlockAbstract.h"
#define BLOCK_INDEX 0
#define BLOCK_NAME "testBlock"
#define BLOCK_LABEL "Test Block"
#define LANGUAGE_NAME "test"
#define LANGUAGE_LABEL "Test"
#define PROPERTY1 "property1"
#define PROPERTY2 "property2"
namespace Test {
namespace Dummy {




/*!
 * This is a dummy test class. It implements the abstract block class for unit
 * testing.
 */
class Block:
    public ::Block::Abstract
{
    Q_OBJECT
    public:
    int addCount {0};
    int lastAddIndex {-1};
    int lastFromMoveIndex {-1};
    int lastRemoveIndex {-1};
    int lastToMoveIndex {-1};
    int moveCount {0};
    int removeCount {0};
    QString property1;
    QString property2;
    using Abstract::Abstract;
    virtual Widget::Block::Abstract* createWidget() const override final
    {
        return nullptr;
    }
    virtual QIcon displayIcon() const override final
    {
        return QIcon();
    }
    virtual QString displayText() const override final
    {
        return QString();
    }
    virtual QString fileName() const override final
    {
        return qobject_cast<Block*>(parent())? property1 : "ROOT";
    }
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override final
    {
        Q_UNUSED(version);
        property1 = map.value(PROPERTY1).toString();
        property2 = map.value(PROPERTY2).toString();
    }
    virtual QMap<QString,QVariant> saveToMap() const override final
    {
        return {{PROPERTY1,property1},{PROPERTY2,property2}};
    }
    virtual void setState(
        const QHash<QString,QVariant>& state
    ) override final
    {
        Q_UNUSED(state);
    }
    virtual QHash<QString,QVariant> state() const override final
    {
        return {};
    }
    virtual void addEvent(
        int index
    ) override final
    {
        addCount++;
        lastAddIndex = index;
    }
    virtual Abstract* create(
        QObject* parent = nullptr
    ) const override final
    {
        return new Block(meta(),parent);
    }
    virtual void moveEvent(
        int from
        ,int to
    ) override final
    {
        moveCount++;
        lastFromMoveIndex = from;
        lastToMoveIndex = to;
    }
    virtual void removeEvent(
        int index
    ) override final
    {
        removeCount++;
        lastRemoveIndex = index;
    }
};
}
}


#endif
