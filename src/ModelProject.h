#ifndef MODEL_PROJECT_H
#define MODEL_PROJECT_H
#include <QAbstractItemModel>
#include "Block.h"
#include "Language.h"
class QXmlStreamReader;
namespace Model {




/*!
 * This is a model class. It represents a Socrates' Reference project.
 * 
 * Its properties are language, name, directory path, and parse path. The
 * language and name are self-explanatory. The directory path is where a
 * project's save file is located. The parse path is the path, relative to the
 * location of a project's directory path, where a project's source code is
 * contained.
 */
class Project:
    public QAbstractItemModel
{
    Q_OBJECT
    Block::Abstract* _root {nullptr};
    Language::Abstract* _language {nullptr};
    QString _directoryPath;
    QString _name;
    QString _parsePath;


    /*!
     * Constructs this new model with the given path and parent. This model
     * loads its contents from the XML file at the given path.
     * 
     * A Qt string is thrown if any error is encountered.
     *
     * @param path
     *        The path.
     *
     * @param parent
     *        The parent.
     */
    public:
    Project(
        const QString& path
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
     * Signals this model's parse path property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void parsePathChanged(
        const QString& value
    );


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
    );


    public:
    virtual QModelIndex index(
        int row
        ,int column
        ,const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * This model's language property.
     */
    public:
    Language::Abstract* language(
    );


    /*!
     * Getter method.
     *
     * @return
     * This model's name property.
     */
    public:
    const QString& name(
    );


    public:
    virtual QModelIndex parent(
        const QModelIndex& index
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * This model's parse path property.
     */
    public:
    const QString& parsePath(
    );


    public:
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()
    ) const override final;


    /*!
     * Sets this model's directory path property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setDirectoryPath(
        const QString& value
    );


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
     * Sets this model's parse path property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setParsePath(
        const QString& value
    );


    /*!
     * Called when this project's language's destroyed signal is emitted.
     */
    private slots:
    void onLanguageDestroyed(
        QObject* object
    );


    /*!
     * Loads this model from the given path.
     * 
     * A Qt string is thrown if any error is encountered.
     *
     * @param path
     *        The path.
     */
    private:
    void read(
        const QString& path
    );


    /*!
     * Loads this model from the given XML reader, assuming the XML format is
     * legacy.
     * 
     * A Qt string is thrown if any error is encountered.
     *
     * @param xml
     *        The XML reader.
     */
    private:
    void readLegacy(
        QXmlStreamReader& xml
    );
};
}


#endif
