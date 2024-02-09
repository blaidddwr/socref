#ifndef LANGUAGE_ABSTRACT_H
#define LANGUAGE_ABSTRACT_H
#include <QObject>
#include <QHash>
#include "Block.h"
#include "ModelMeta.h"
namespace Language {




/*!
 * This is an abstract class. It is a language implementation and block factory
 * for its implemented language. A language in this application is synonymous
 * with a programming language, such as C++ or Python for example.
 * 
 * Its properties are meta and root index. Meta is self-explanatory. Root index
 * is a language's root block's index.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    Model::Meta::Language* _meta;
    QHash<QString,int> _lookup;
    QList<Model::Meta::Block*> _blocks;


    /*!
     * Constructs this new language with the given meta and parent. The given
     * meta must be valid and cannot be destroyed during the life of this
     * language instance.
     *
     * @param meta
     *        The meta.
     *
     * @param parent
     *        The parent.
     */
    public:
    Abstract(
        Model::Meta::Language* meta
        ,QObject* parent = nullptr
    );


    /*!
     * Returns this language's block's meta at the given index. The given index
     * must be valid.
     *
     * @param index
     *        The index.
     */
    public:
    Model::Meta::Block* blockMeta(
        int index
    ) const;


    /*!
     * Creates and returns a new block of this language with the given index and
     * parent. The given index must be valid.
     *
     * @param index
     *        The index.
     *
     * @param parent
     *        The parent.
     */
    public:
    virtual Block::Abstract* create(
        int index
        ,QObject* parent = nullptr
    ) const = 0;


    /*!
     * Creates and returns a new root block of this language with the given
     * parent.
     *
     * @param parent
     *        The parent.
     */
    public:
    virtual Block::Abstract* createRoot(
        QObject* parent = nullptr
    ) const = 0;


    /*!
     * Returns this language's block index with the given meta name. If there is
     * no such block with the given meta name then -1 is returned.
     *
     * @param name
     *        The name.
     */
    public:
    int indexFromName(
        const QString& name
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This language's meta property.
     */
    public:
    Model::Meta::Language* meta(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This language's root index property.
     */
    public:
    virtual int rootIndex(
    ) const = 0;


    /*!
     * Getter method.
     *
     * @return
     * This language's total number of block implementations.
     */
    public:
    int size(
    ) const;


    /*!
     * Appends the given block meta to this language's list of block meta. This
     * language takes ownership of the given meta. This language's create
     * interface implementation must match the order generated from calling this
     * method in regard to block indexes.
     * 
     * The given meta must be valid. Its name must be unique among all other
     * block meta in this language. Its name cannot be an empty string. Its name
     * cannot begin with an underscore character.
     *
     * @param meta
     *        The block meta.
     */
    protected:
    void appendBlock(
        Model::Meta::Block* meta
    );


    /*!
     * Called when this language's meta's destroyed signal is emitted.
     */
    private slots:
    void onMetaDestroyed(
        QObject* object
    );
};
}


#endif
