#ifndef TESTBLOCK_H
#define TESTBLOCK_H
#include <QtGui>
#include "AbstractBlock.h"
#define BLOCK_INDEX 0
#define BLOCK_NAME "testBlock"
#define BLOCK_LABEL "Test Block"
#define LANGUAGE_NAME "test"
#define LANGUAGE_LABEL "Test"
#define PROPERTY1 "property1"
#define PROPERTY2 "property2"

class TestBlock: public AbstractBlock
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
    using AbstractBlock::AbstractBlock;
    virtual AbstractBlockWidget* createWidget() const override final
    {
        return nullptr;
    }
    virtual QIcon displayIcon() const override final
    {
        return QIcon(":/application.svg");
    }
    virtual QString displayText() const override final
    {
        return property1;
    }
    virtual QString fileName() const override final
    {
        return qobject_cast<TestBlock*>(parent())? property1 : "ROOT";
    }
    virtual void loadFromMap(const QMap<QString,QVariant>& map, int version) override final
    {
        Q_UNUSED(version);
        property1 = map.value(PROPERTY1).toString();
        property2 = map.value(PROPERTY2).toString();
    }
    virtual QMap<QString,QVariant> saveToMap() const override final
    {
        return {{PROPERTY1,property1},{PROPERTY2,property2}};
    }
    virtual void setState(const QHash<QString,QVariant>& state) override final
    {
        property1 = state.value("property1").toString();
        property2 = state.value("property2").toString();
    }
    virtual QHash<QString,QVariant> state() const override final
    {
        QHash<QString,QVariant> ret;
        ret.insert("property1",property1);
        ret.insert("property2",property2);
        return ret;
    }
    virtual void addEvent(int index) override final
    {
        addCount++;
        lastAddIndex = index;
    }
    virtual AbstractBlock* create(QObject* parent = nullptr) const override final
    {
        return new TestBlock(meta(),parent);
    }
    virtual void moveEvent(int from, int to) override final
    {
        moveCount++;
        lastFromMoveIndex = from;
        lastToMoveIndex = to;
    }
    virtual void removeEvent(int index) override final
    {
        removeCount++;
        lastRemoveIndex = index;
    }
};

#endif
