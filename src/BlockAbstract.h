#ifndef BLOCK_ABSTRACT_H
#define BLOCK_ABSTRACT_H
#include <QObject>
#include "Language.h"
#include "ModelMeta.h"
#include "WidgetBlock.h"
class QXmlStreamReader;
class QXmlStreamWriter;
namespace Block {




/*!
 * This is an abstract class. It is a single element, or block, of source code
 * for a language.
 * 
 * A block represents an atomic element of a programming language such as a
 * variable, function, or class. Blocks can contain any number of children and
 * have only one parent. The only block that does not have a parent is the root
 * block of a project. One and only one special block type must be assigned as
 * the root block type for each implemented language. Blocks form tree
 * relationships such as a variable block being the child of a function making
 * it an argument of the function or a function being a child of a class making
 * it a method of the class.
 * 
 * Its properties are meta, display text, and display icon. Meta, display text,
 * and display icon are self-explanatory.
 * 
 * A block can only have children blocks of any type in its meta's allow list
 * property.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    Model::Meta::Block* _meta;
    QList<Abstract*> _children;


    /*!
     * Constructs this new block with the given meta and parent. The given meta
     * must be valid and cannot be destroyed during the life of this block
     * instance.
     *
     * @param meta
     *        The meta.
     *
     * @param parent
     *        The parent.
     */
    public:
    Abstract(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    /*!
     * Signals this block's display icon property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void displayIconChanged(
        const QIcon& value
    );


    /*!
     * Signals this block's display text property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void displayTextChanged(
        const QString& value
    );


    /*!
     * Appends the given block to this block's list of block children. This
     * block takes ownership of the given block. The given block must be valid,
     * its meta index in this block's meta allow list, and not already a child
     * of this block.
     *
     * @param block
     *        The block.
     */
    public:
    void append(
        Block::Abstract* block
    );


    /*!
     * Creates and returns a block widget for this block with the given parent.
     *
     * @param parent
     *        The parent.
     */
    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const = 0;


    /*!
     * Getter method.
     *
     * @return
     * This block's display icon property.
     */
    public:
    virtual QIcon displayIcon(
    ) const = 0;


    /*!
     * Getter method.
     *
     * @return
     * This block's display text property.
     */
    public:
    virtual QString displayText(
    ) const = 0;


    /*!
     * Creates and returns a new block and all its children with the given
     * language, XML format version, XML reader, and parent. The given language
     * must be valid.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The XML format version.
     *
     * @param xml
     *        The XML reader.
     *
     * @param parent
     *        The parent.
     */
    public:
    static Block::Abstract* fromXml(
        Language::Abstract* language
        ,int version
        ,QXmlStreamReader& xml
        ,QObject* parent = nullptr
    );


    /*!
     * Returns this block's child block at the given index. The given index must
     * be valid.
     *
     * @param index
     *        The index.
     */
    public:
    Block::Abstract* get(
        int index
    ) const;


    /*!
     * Returns the index of the given block assuming it is a child of this
     * block. If the given block is not a child of this block then -1 is
     * returned.
     *
     * @param block
     *        The block.
     */
    public:
    int indexOf(
        Block::Abstract* block
    );


    /*!
     * Inserts the given block to this block's list of block children at the
     * given index. This block takes ownership of the given block. The given
     * block must be valid, its meta index in this block's meta allow list, and
     * not already a child of this block.
     * 
     * The given index can go beyond the bounds of this block's children list.
     * If it is less than 0 then it is inserted at the beginning. If it is
     * greater or equal to the children list size then it is appended to the
     * end.
     *
     * @param index
     *        The index.
     *
     * @param block
     *        The block.
     */
    public:
    void insert(
        int index
        ,Block::Abstract* block
    );


    /*!
     * Sets this block's data from the given mapping using the given format
     * version, overwriting any data contained in this block.
     *
     * @param map
     *        The mapping.
     *
     * @param version
     *        The XML format version.
     */
    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) = 0;


    /*!
     * Getter method.
     *
     * @return
     * This block's meta property.
     */
    public:
    Model::Meta::Block* meta(
    ) const;


    /*!
     * Saves this blocks data to a returned mapping.
     */
    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const = 0;


    /*!
     * Getter method.
     *
     * @return
     * This block's total number of children blocks.
     */
    public:
    int size(
    ) const;


    /*!
     * Takes this block's child block at the given index, removing it from this
     * block's list of children and returning it. This also sets the returned
     * block's parent to null. The given index must be valid.
     *
     * @param index
     *        The index.
     */
    public:
    Block::Abstract* take(
        int index
    );


    /*!
     * Writes this block and all its children to the given XML writer using the
     * most current XML format version.
     *
     * @param xml
     *        The XML writer.
     */
    public:
    void toXml(
        QXmlStreamWriter& xml
    );


    /*!
     * Called right after this block has been added as a child to its new parent
     * block.
     */
    protected:
    virtual void addEvent(
    );


    /*!
     * Called right before it is removed as a child from its current parent
     * block.
     */
    protected:
    virtual void removeEvent(
    );


    /*!
     * Called when this block's meta block model's destroyed signal is emitted.
     */
    private slots:
    void onMetaDestroyed(
        QObject* object
    );
};
}


#endif
