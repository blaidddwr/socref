#ifndef MODEL_PROJECT_H
#define MODEL_PROJECT_H
#include <QAbstractItemModel>
#include "Block.h"
#include "CommandProject.h"
#include "Language.h"
#include "Stream.h"
class QXmlStreamReader;
namespace Model {




/*!
 * This is a model class. It represents a Socrates' Reference project.
 * 
 * Its properties are language, name, directory path, relative code path, and
 * modified. The language and name are self-explanatory. The directory path is
 * where a project's files are located. The relative code path is the path,
 * relative to the location of a project's directory path, where a project's
 * source code is contained. The modified flag is self-explanatory.
 */
class Project:
    public QAbstractItemModel
{
    Q_OBJECT
    friend class Command::Project::Abstract;
    friend class Command::Project::Insert;
    friend class Command::Project::Move;
    friend class Command::Project::Remove;
    friend class Command::Project::Set;
    friend class Stream::Project;
    Block::Abstract* _root {nullptr};
    Language::Abstract* _language {nullptr};
    QHash<QString,QVariant> _previousState;
    QList<Command::Project::Abstract*> _redoStack;
    QList<Command::Project::Abstract*> _undoStack;
    QPersistentModelIndex _setIndex;
    QString _directoryPath;
    QString _name;
    QString _relativeCodePath;
    bool mutable _modified {true};
    static QList<Block::Abstract*> _copied;


    /*!
     * Signals this model's directory path property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void directoryPathChanged(
        const QString& value
    );


    /*!
     * Signals this model's modified property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void modifiedChanged(
        bool value
    );


    /*!
     * Signals this model's name property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void nameChanged(
        const QString& value
    );


    /*!
     * Signals this instance's relative code path property has changed to the
     * given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void relativeCodePathChanged(
        const QString& value
    );


    /*!
     * Constructs this new instance with the given language index and parent.
     * 
     * This instance is a new project.
     * 
     * The given language index must be valid.
     *
     * @param languageIndex
     *        The language index.
     *
     * @param parent
     *        The parent.
     */
    public:
    Project(
        int languageIndex
        ,QObject* parent = nullptr
    );


    /*!
     * Informs this model the block given in the last start set call failed in
     * changing its state, therefore not making any state change to the block.
     * 
     * The begin set method must be called before this method.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool abortSet(
    );


    /*!
     * Returns this project's absolute code path, derived from its directory
     * path and relative code path.
     */
    public:
    QString absoluteCodePath(
    ) const;


    /*!
     * Informs this model that the block at the given index is about to have its
     * state changed.
     * 
     * This must be called before the finish or abort set methods. Once this
     * method is called it cannot be called again until the finish or abort set
     * method is called to finish the set operation. The given index must be
     * valid.
     *
     * @param index
     *        The index.
     *
     * @return
     * The block this is about to be set on success or null otherwise.
     */
    public:
    Block::Abstract* beginSet(
        const QModelIndex& index
    );


    /*!
     * Returns the block index of this model's block at the given Qt model
     * index.
     *
     * @param index
     *        The Qt model index.
     */
    public:
    int blockIndex(
        const QModelIndex& index
    ) const;


    /*!
     * Determines if this model's block in the given parent index can be moved
     * from the given from index to the given to index.
     *
     * @param parent
     *        The parent index.
     *
     * @param from
     *        The from row.
     *
     * @param to
     *        The to row.
     *
     * @return
     * True if it can be moved or false otherwise.
     */
    public:
    bool canMove(
        const QModelIndex& parent
        ,int from
        ,int to
    ) const;


    /*!
     * Returns the number of copied blocks that can be pasted into this model at
     * the given parent index.
     *
     * @param parent
     *        The parent index.
     */
    public:
    int canPaste(
        const QModelIndex& parent
    ) const;


    /*!
     * Determines if there is an undone command in this project model that can
     * be redone.
     *
     * @return
     * True if there is a command that can be redone or false otherwise.
     */
    public:
    bool canRedo(
    ) const;


    /*!
     * Determines if there is a done command in this project model that can be
     * undone.
     *
     * @return
     * True if there is a command that can be undone or false otherwise.
     */
    public:
    bool canUndo(
    ) const;


    public:
    virtual int columnCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Returns the block contained in this model at the given index.
     * 
     * The given index must be valid.
     * 
     * A constant pointer is returned because the block cannot be modified or
     * deleted. Doing either action will corrupt this project model and
     * potentially crash the application.
     *
     * @param index
     *        The index.
     */
    public:
    const Block::Abstract* constBlock(
        const QModelIndex& index
    ) const;


    /*!
     * Copies the blocks in this model at the given indexes.
     *
     * @param indexes
     *        The indexes.
     *
     * @return
     * The number of blocks copied.
     */
    public:
    int copy(
        const QModelIndexList& indexes
    ) const;


    /*!
     * Cuts the blocks in this model at the given indexes.
     *
     * @param indexes
     *        The indexes.
     *
     * @return
     * The number of blocks cut.
     */
    public:
    int cut(
        const QModelIndexList& indexes
    );


    public:
    virtual QVariant data(
        const QModelIndex& index
        ,int role = Qt::DisplayRole
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * This model's directory path property.
     */
    public:
    const QString& directoryPath(
    ) const;


    /*!
     * Informs this model the block given in the last start set call has
     * successfully had its state changed.
     * 
     * The finish set method must be called before this method.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool finishSet(
    );


    public:
    virtual QVariant headerData(
        int section
        ,Qt::Orientation orientation
        ,int role
    ) const override final;


    public:
    virtual QModelIndex index(
        int row
        ,int column
        ,const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Inserts a new block with the given block index into this model at the
     * given row in the given parent.
     * 
     * The given block index and row must be valid.
     *
     * @param blockIndex
     *        The block index.
     *
     * @param row
     *        The row.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool insert(
        int blockIndex
        ,int row
        ,const QModelIndex& parent
    );


    /*!
     * Getter method.
     *
     * @return
     * This model's language property.
     */
    public:
    Language::Abstract* language(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This model's modified property.
     */
    public:
    bool modified(
    ) const;


    /*!
     * Moves the child block of the given parent index from the given from row
     * to the given to row.
     * 
     * The given from and to rows must produce a valid move operation in this
     * model.
     *
     * @param parent
     *        The parent index.
     *
     * @param from
     *        The from row.
     *
     * @param to
     *        The to row.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool move(
        const QModelIndex& parent
        ,int from
        ,int to
    );


    /*!
     * Getter method.
     *
     * @return
     * This model's name property.
     */
    public:
    const QString& name(
    ) const;


    public:
    virtual QModelIndex parent(
        const QModelIndex& index
    ) const override final;


    /*!
     * Pastes the copied blocks into this model within the given parent index at
     * the given row. If any copied block cannot be a child block of the given
     * parent index block then it is ignored.
     *
     * @param parent
     *        The parent index.
     *
     * @param row
     *        The row.
     *
     * @return
     * The number of blocks pasted.
     */
    public:
    int paste(
        const QModelIndex& parent
        ,int row
    );


    /*!
     * Redoes the last command undone on this project model.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool redo(
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's relative code path property.
     */
    public:
    const QString& relativeCodePath(
    ) const;


    /*!
     * Removes the blocks at the given indexes in this model.
     *
     * @param indexes
     *        The indexes.
     *
     * @return
     * The number of blocks removed.
     */
    public:
    int remove(
        const QModelIndexList& indexes
    );


    public:
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Sets this model's name property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setName(
        const QString& value
    );


    /*!
     * Sets this instance's relative code path property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setRelativeCodePath(
        const QString& value
    );


    /*!
     * Undoes the last command done or redone on this project model.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool undo(
    );


    private:
    Project(
    ) = default;


    /*!
     * Returns the block contained in this model at the given index.
     * 
     * If the given index is invalid then the root block is returned, else if
     * the given index is valid then it must be derived from this model.
     *
     * @param index
     *        The index.
     */
    private:
    Block::Abstract* block(
        const QModelIndex& index
    ) const;


    /*!
     * Detailed description.
     */
    private:
    void connectAll(
    );


    /*!
     * Called when the given block in this model's display icon changed signal
     * is emitted.
     * 
     * The given block must be valid.
     *
     * @param block
     *        The block.
     */
    private slots:
    void onBlockDisplayIconChanged(
        Block::Abstract* block
    );


    /*!
     * Called when the given block in this model's display text changed signal
     * is emitted.
     * 
     * The given block must be valid.
     *
     * @param block
     *        The block.
     */
    private slots:
    void onBlockDisplayTextChanged(
        Block::Abstract* block
    );


    /*!
     * Called when this project's language's destroyed signal is emitted.
     */
    private slots:
    void onLanguageDestroyed(
        QObject* object
    );


    /*!
     * Pushes the given command onto this model's undo stack and calls its redo
     * interface beforehand.
     * 
     * This does not take ownership of the given command.
     * 
     * If the called redo interface returns false then this does not push the
     * given command onto this model's undo stack and deletes the given command.
     * 
     * The given command must be valid.
     *
     * @param command
     *        The command.
     *
     * @return
     * True if the command was pushed or false otherwise.
     */
    private:
    bool pushCommand(
        Command::Project::Abstract* command
    );


    /*!
     * Sets this instance's directory path property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setDirectoryPath(
        const QString& value
    );


    /*!
     * Sets this model's modified property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setModified(
        bool value
    );
};
}


#endif
