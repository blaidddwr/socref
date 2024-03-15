#ifndef STREAM_PROJECTDIR_H
#define STREAM_PROJECTDIR_H
#include <QDir>
#include "Model.h"
namespace Stream {




/*!
 * This is a stream class. It handles all input and output of directories for
 * project model class instances.
 * 
 * Its single property is path. The path is used as the location of the
 * directory where all loading and saving is done.
 */
class ProjectDir
{
    QDir _dir;


    /*!
     * Constructs this new stream with the given path.
     *
     * @param path
     *        The path.
     */
    public:
    ProjectDir(
        const QString& path
    );


    /*!
     * A write project, logical project, write block, or file system exception
     * is thrown if any error is encountered.
     *
     * @param project
     *        The project.
     */
    public:
    ProjectDir& operator<<(
        Model::Project& project
    );


    /*!
     * Loads a project with the given parent.
     * 
     * A read project, read block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The loaded project.
     */
    public:
    Model::Project* load(
        QObject* parent = nullptr
    ) const;


    /*!
     * Returns a list of orphaned block file paths not used by the given project
     * in its directory.
     * 
     * The returned file paths are relative to the given project's directory
     * path.
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
     * Removes orphaned block files at the given paths.
     * 
     * The given paths are relative to the given project's directory path. All
     * given paths are verified to be an orphaned block file for the given
     * project.
     * 
     * If the git flag is true then the git command is used for removing
     * deprecated block files, else the standard system remove command is used.
     * 
     * A logical project, logical block, file system, or run system exception is
     * thrown if any error is encountered.
     *
     * @param paths
     *        The paths.
     *
     * @param project
     *        The project.
     *
     * @param git
     *        The git flag.
     */
    public:
    void removeOrphanFiles(
        const QStringList& paths
        ,const Model::Project& project
        ,bool git = false
    ) const;


    /*!
     * Reads in the configuration file to the given project.
     * 
     * A read project or file system exception is thrown if any error is
     * encountered.
     *
     * @param project
     *        The project.
     *
     * @return
     * The version.
     */
    private:
    int read(
        Model::Project& project
    ) const;


    /*!
     * Writes out the configuration file of the given project.
     * 
     * A file system or write project exception is thrown if any error is
     * encountered.
     *
     * @param project
     *        The project.
     */
    private:
    void write(
        const Model::Project& project
    ) const;
};
}


#endif
