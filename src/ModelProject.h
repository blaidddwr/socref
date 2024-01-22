#ifndef MODEL_PROJECT_H
#define MODEL_PROJECT_H
#include <QAbstractItemModel>
#include "Block.h"
#include "CommandProject.h"
#include "Language.h"
class QXmlStreamReader;
namespace Model {




/*!
 * This is a model class. It represents a Socrates' Reference project.
 * 
 * Its properties are language, name, directory path, and relative parse path.
 * The language and name are self-explanatory. The directory path is where a
 * project's files are located. The parse path is the path, relative to the
 * location of a project's directory path, where a project's source code is
 * contained.
 */
class Project:
    public QAbstractItemModel
{
    Q_OBJECT
    friend class Command::Project::Abstract;
    friend class Command::Project::Insert;
    friend class Command::Project::Move;
    Block::Abstract* _root {nullptr};
    Language::Abstract* _language {nullptr};
    QList<Command::Project::Abstract*> _redoStack;
    QList<Command::Project::Abstract*> _undoStack;
    QString _directoryPath;
    QString _name;
    QString _relativeParsePath;


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
     * Constructs this new project with the given directory path and parent.
     * This project's data and its block's data is loaded from the given
     * directory path.
     * 
     * A read project or file system exception are thrown if any error is
     * encountered.
     *
     * @param directoryPath
     *        The directory path.
     *
     * @param parent
     *        The parent.
     */
    public:
    Project(
        const QString& directoryPath
        ,QObject* parent = nullptr
    );


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
     * Signals this instance's relative parse path property has changed to the
     * given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void relativeParsePathChanged(
        const QString& value
    );


    /*!
     * Returns this project's absolute parse path, derived its directory path
     * and relative parse path.
     */
    public:
    QString absoluteParsePath(
    ) const;


    /*!
     * Returns the block contained in this model at the given index.
     * 
     * If the given index is invalid then the root block is returned, else if
     * the given index is valid then it must be derived from this model.
     *
     * @param index
     *        The index.
     */
    public:
    Block::Abstract* block(
        const QModelIndex& index
    ) const;


    public:
    virtual int columnCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


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
     * Creates and returns a new project model with the given parent imported
     * from an exported XML file located at the given path.
     * 
     * A read project or file system exception are thrown if any error is
     * encountered.
     *
     * @param path
     *        The path.
     *
     * @param parent
     *        The parent.
     */
    public:
    static Model::Project* import(
        const QString& path
        ,QObject* parent = nullptr
    );


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
     * Getter method.
     *
     * @return
     * This instance's relative parse path property.
     */
    public:
    const QString& relativeParsePath(
    ) const;


    /*!
     * Removes the block at the given index in this model.
     * 
     * The given index must be valid.
     *
     * @param index
     *        The index.
     *
     * @return
     * True on success or false otherwise.
     */
    public:
    bool remove(
        const QModelIndex& index
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
     * Sets this instance's relative parse path property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setRelativeParsePath(
        const QString& value
    );


    private:
    Project(
    ) = default;


    /*!
     * Called when this project's language's destroyed signal is emitted.
     */
    private slots:
    void onLanguageDestroyed(
        QObject* object
    );


    /*!
     * Reads in this model's data and its block's data from the multi-file
     * project directory at the given path.
     * 
     * A read project or file system exception are thrown if any error is
     * encountered.
     *
     * @param path
     *        The path.
     */
    private:
    void readDir(
        const QString& path
    );


    /*!
     * Reads in this model's data from the multi-file directory configuration
     * file at the given path.
     * 
     * A read project exception is thrown if any error is encountered.
     *
     * @param path
     *        The path.
     */
    private:
    void readDirConfig(
        const QString& path
    );


    /*!
     * Reads in this model's data and its block's data from the exported XML
     * file at the given path.
     * 
     * A read project or file system exception are thrown if any error is
     * encountered.
     *
     * @param path
     *        The path.
     */
    private:
    void readXml(
        const QString& path
    );


    /*!
     * Reads in this model's data and its block's data from the given XML reader
     * using the legacy format. The given XML reader's current token position
     * must be the start element of the project.
     * 
     * A read project exception is thrown if any error is encountered.
     *
     * @param xml
     *        The XML reader.
     */
    private:
    void readXmlLegacy(
        QXmlStreamReader& xml
    );
};
}


#endif
