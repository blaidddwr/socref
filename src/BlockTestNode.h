#ifndef BLOCK_TEST_NODE_H
#define BLOCK_TEST_NODE_H
#include "BlockAbstract.h"
class QIcon;
namespace Block {
namespace Test {




/*!
 * This is a test block class. It represents no real language component or
 * object, instead being used by this application's test system.
 * 
 * Its single property is name. The name is self-explanatory.
 * 
 * Nodes have an add, remove, and move count. This keeps track of how many times
 * the add and remove events have been called by its parent abstract block
 * class. It also has getter methods for getting the last indexes of said
 * interfaces.
 * 
 * The display icon property can be set with an appropriate setter method so it
 * can be unit tested with abstract block.
 */
class Node:
    public Block::Abstract
{
    Q_OBJECT
    QIcon* _icon {nullptr};
    QString _name;
    int _addCount {0};
    int _lastAddIndex {-1};
    int _moveCount {0};
    int _lastMoveFromIndex {-1};
    int _lastMoveToIndex {-1};
    int _lastRemoveIndex {-1};
    int _removeCount {0};
    public:
    using Block::Abstract::Abstract;


    public:
    virtual ~Node(
    ) override;


    /*!
     * Getter method.
     *
     * @return
     * This node's add count.
     */
    public:
    int addCount(
    ) const;


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
     * The index argument from the last add event interface call.
     */
    public:
    int lastAddIndex(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The from index argument from the last move event interface call.
     */
    public:
    int lastMoveFromIndex(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The to index argument from the last move event interface call.
     */
    public:
    int lastMoveToIndex(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The index argument from the last remove event interface call.
     */
    public:
    int lastRemoveIndex(
    ) const;


    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override final;


    /*!
     * Getter method.
     *
     * @return
     * This node's move count.
     */
    public:
    int moveCount(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This block's name property.
     */
    public:
    const QString& name(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This node's remove count.
     */
    public:
    int removeCount(
    ) const;


    /*!
     * Resets this node's add count to 0.
     */
    public:
    void resetAdd(
    );


    /*!
     * Resets this node's move count to 0.
     */
    public:
    void resetMove(
    );


    /*!
     * Resets this node's remove count to 0.
     */
    public:
    void resetRemove(
    );


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    public:
    virtual QString scope(
    ) const override final;


    /*!
     * Sets this instance's display icon property to the given value.
     *
     * @param icon
     *        The value.
     */
    public:
    void setIcon(
        const QIcon& icon
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


    protected:
    virtual void addEvent(
        int index
    ) override final;


    protected:
    virtual void moveEvent(
        int from
        ,int to
    ) override final;


    protected:
    virtual void removeEvent(
        int index
    ) override final;
};
}
}


#endif
