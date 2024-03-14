#ifndef STREAM_PROJECT_H
#define STREAM_PROJECT_H
#include <QObject>
#include "Model.h"
class QXmlStreamReader;
namespace Stream {




/*!
 * This is a stream class. It is a singleton class. It handles all input and
 * output for project model class instances.
 */
class Project:
    public QObject
{
    Q_OBJECT
    static Project* _instance;


    /*!
     * Reads in a project from a multi block file directory at the given path.
     * The project model's parent is set to the given parent.
     * 
     * A read project, read block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param path
     *        The path.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The project model.
     */
    public:
    Model::Project* fromDir(
        const QString& path
        ,QObject* parent = nullptr
    ) const;


    /*!
     * Reads in a project from a XML file at the given path. The project model's
     * parent is set to the given parent.
     * 
     * A read project, read block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param path
     *        The path.
     *
     * @param parent
     *        The parent.
     */
    public:
    Model::Project* fromXml(
        const QString& path
        ,QObject* parent = nullptr
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The singleton instance of this class.
     */
    public:
    static Project* instance(
    );


    /*!
     * Returns a list of orphaned block files not used by the given project in
     * its multi block file directory. The given project must have a valid
     * directory path.
     * 
     * A logical project or file system exception is thrown if any error is
     * encountered.
     *
     * @param project
     *        The project.
     */
    public:
    QStringList orphanFiles(
        const Model::Project& project
    ) const;


    /*!
     * Removes orphaned block files at the given file paths.
     * 
     * All given file paths are verified to be an orphaned block file for the
     * given project.
     * 
     * If the git flag is true then the git command is used for removing
     * deprecated block files, else the standard system remove command is used.
     * 
     * A logical project, logical block, file system, or run system exception is
     * thrown if any error is encountered.
     *
     * @param filePaths
     *        The file paths.
     *
     * @param project
     *        The project.
     *
     * @param git
     *        The git flag.
     */
    public:
    void removeOrphanFiles(
        const QStringList& filePaths
        ,const Model::Project& project
        ,bool git = false
    ) const;


    /*!
     * Writes out the given project to a multi block file directory at the
     * project's directory path. The given project must have a valid directory
     * path.
     * 
     * A write project, logical project, write block, or file system exception
     * is thrown if any error is encountered.
     *
     * @param project
     *        The project.
     */
    public:
    void toDir(
        Model::Project& project
    ) const;


    /*!
     * Writes out the given project to a multi block file directory at the given
     * path. The given project's directory is set to the given path if no
     * exception occurs.
     * 
     * A write project, write block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param project
     *        The project.
     *
     * @param path
     *        The path.
     */
    public:
    void toDir(
        Model::Project& project
        ,const QString& path
    ) const;


    /*!
     * Writes out the given project to a XML file at the given path.
     * 
     * A write project, write block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param project
     *        The project.
     *
     * @param path
     *        The path.
     */
    public:
    void toXml(
        const Model::Project& project
        ,const QString& path
    ) const;


    private:
    Project(
        QObject* parent = nullptr
    );


    /*!
     * Reads in the configuration file at the given path to the given project.
     * 
     * A read project or file system exception is thrown if any error is
     * encountered.
     *
     * @param path
     *        The path.
     *
     * @param project
     *        The project.
     *
     * @return
     * The format version.
     */
    private:
    int read(
        const QString& path
        ,Model::Project& project
    ) const;


    /*!
     * Writes out the given project to a multi block file directory at the given
     * path.
     * 
     * A write project, write block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param project
     *        The project.
     *
     * @param path
     *        The path.
     */
    private:
    void saveDir(
        const Model::Project& project
        ,const QString& path
    ) const;


    /*!
     * Writes out the configuration file of the given project to the given path.
     * 
     * A file system or write project exception is thrown if any error is
     * encountered.
     *
     * @param project
     *        The project.
     *
     * @param path
     *        The path.
     */
    private:
    void write(
        const Model::Project& project
        ,const QString& path
    ) const;
};
}


#endif
