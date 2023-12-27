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
 * Its properties are meta, display text, display icon, and scope. Meta, display
 * text, and display icon are self-explanatory.
 * 
 * The scope is a that must be unique among all other blocks that share a common
 * root block making up a project. The scope cannot contain non printable
 * characters such as new lines. The root block of a project must have a scope
 * of "ROOT".
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
     * A complete list of all descendant blocks contained in this block.
     */
    public:
    QList<Block::Abstract*> descendants(
    ) const;


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
     * Creates and returns a new root block and all its children located in the
     * given directory path with the given language, format version, and parent.
     * The given language must be valid.
     * 
     * It is expected the given directory contains a valid multi-file project
     * with the root block file named "ROOT.srb".
     * 
     * A read block or file system exception is thrown if any error is
     * encountered.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The format version.
     *
     * @param path
     *        The directory path.
     *
     * @param parent
     *        The parent.
     */
    public:
    static Block::Abstract* fromDir(
        Language::Abstract* language
        ,int version
        ,const QString& path
        ,QObject* parent = nullptr
    );


    /*!
     * Creates and returns a new block and all its children with the given
     * language, format version, XML reader, and parent. The given language must
     * be valid.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The format version.
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
        const Block::Abstract* block
    ) const;


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
     * A read block exception is thrown if any error is encountered.
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
     * Moves this block's child at the given from index to the given to index.
     * The given from and to indexes must be valid.
     *
     * @param from
     *        The from index.
     *
     * @param to
     *        The to index.
     */
    public:
    void move(
        int from
        ,int to
    );


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
     * This instance's scope property.
     */
    public:
    virtual QString scope(
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
     * Writes this block and all its children to the given directory path using
     * the most current format version of the multi-file system. This must be a
     * root block.
     * 
     * Any SRB files that do not match any of this root block's and its
     * descendant's SRB files are removed from the file system within the given
     * directory.
     * 
     * A logical block or file system exception is thrown if an error is
     * encountered.
     *
     * @param path
     *        The directory path.
     */
    public:
    void toDir(
        const QString& path
    ) const;


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
    ) const;


    /*!
     * Called right after this instance has been added at the given index as a
     * child to its new parent block.
     *
     * @param index
     *        The index.
     */
    protected:
    virtual void addEvent(
        int index
    );


    /*!
     * Called right after this instance has been moved as a child in its parent
     * from the given from index to the given to index.
     *
     * @param from
     *        The from index.
     *
     * @param to
     *        The to index.
     */
    protected:
    virtual void moveEvent(
        int from
        ,int to
    );


    /*!
     * Called right after this instance has been removed at the given index as a
     * child from its current parent block and before its parent is set to null.
     *
     * @param index
     *        The index.
     */
    protected:
    virtual void removeEvent(
        int index
    );


    /*!
     * Called when one of this block's child block's destroyed signal is
     * emitted.
     */
    private slots:
    void onChildDestroyed(
        QObject* object
    );


    /*!
     * Called when this block's meta block model's destroyed signal is emitted.
     */
    private slots:
    void onMetaDestroyed(
        QObject* object
    );


    /*!
     * Creates and returns a new block and all its children located at the given
     * path with the given language, format version, and parent. The given
     * language must be valid.
     * 
     * The new block's children are expected to reside in the same directory as
     * the new block's file's directory.
     * 
     * A read block or file system exception is thrown if any error is
     * encountered.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The format version.
     *
     * @param path
     *        The path.
     *
     * @param parent
     *        The parent.
     */
    private:
    static Block::Abstract* read(
        Language::Abstract* language
        ,int version
        ,const QString& path
        ,QObject* parent = nullptr
    );


    /*!
     * Writes this block's data and children scope links to the SRB file at the
     * given path.
     * 
     * A file system exception is thrown if any error is encountered.
     *
     * @param path
     *        The path.
     */
    private:
    void write(
        const QString& path
    ) const;
};
}


#endif
