#ifndef STREAM_BLOCKDIR_H
#define STREAM_BLOCKDIR_H
#include <QDir>
#include <QSet>
#include "Block.h"
#include "Language.h"
namespace Stream {




/*!
 * This is a stream class. It handles all input and output of directories for
 * abstract block class instances.
 * 
 * Its properties are path, language, and version. The path is used as the
 * location of the directory where all loading and saving is done. The language
 * and version are used when loading blocks.
 */
class BlockDir
{
    Language::Abstract* _language;
    QDir _dir;
    int _version;
    mutable QSet<QString> _registry;
    mutable QStringList _paths;


    /*!
     * Constructs this new stream with the given path, language, and version.
     *
     * @param path
     *        The path.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The version.
     */
    public:
    BlockDir(
        const QString& path
        ,Language::Abstract* language = nullptr
        ,int version = -1
    );


    /*!
     * A write block, logical block, or file system exception is thrown if any
     * error is encountered.
     */
    public:
    BlockDir& operator<<(
        const ::Block::Abstract& block
    );


    /*!
     * Loads a root block with the given parent and all its descendants.
     * 
     * This stream's language and version must be valid.
     * 
     * A read block or file system exception is thrown if any error is
     * encountered.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The loaded root block.
     */
    public:
    ::Block::Abstract* load(
        QObject* parent = nullptr
    ) const;


    /*!
     * Returns a list of orphaned block files not used by the given block and
     * its descendants.
     * 
     * A file system or logical block exception is thrown if any error is
     * encountered.
     *
     * @param block
     *        The block.
     */
    public:
    QStringList orphanFiles(
        const ::Block::Abstract& block
    ) const;


    /*!
     * Removes orphaned block files at the given paths.
     * 
     * All given paths are verified to be an orphaned block file for the given
     * block.
     * 
     * If the git flag is true then the git command is used for removing
     * deprecated block files, else the standard system remove command is used.
     * 
     * A file system, run system, or logical block exception is thrown if any
     * error is encountered.
     *
     * @param paths
     *        The paths.
     *
     * @param block
     *        The block.
     *
     * @param git
     *        The git flag.
     */
    public:
    void removeOrphanFiles(
        const QStringList& paths
        ,const ::Block::Abstract& block
        ,bool git = false
    );


    /*!
     * Inserts the given block's file path, located in the given directory, and
     * all its descendant's file paths to this stream's registry.
     * 
     * A logical block exception is thrown if any error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param dir
     *        The directory.
     */
    private:
    void insertBlockPaths(
        const ::Block::Abstract& block
        ,const QDir& dir
    ) const;


    /*!
     * Inserts all paths of found block files in the given directory into this
     * stream's path list. The given directory is recursively searched for all
     * children block directories.
     *
     * @param dir
     *        The directory.
     */
    private:
    void insertPaths(
        const QDir& dir
    ) const;


    /*!
     * Reads a block and all its children located at the given directory with
     * the given file name and parent.
     * 
     * This stream's language and version must be valid.
     * 
     * A read block or file system exception is thrown if any error is
     * encountered.
     *
     * @param dir
     *        The directory.
     *
     * @param fileName
     *        The file name.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The read in block.
     */
    private:
    ::Block::Abstract* read(
        const QDir& dir
        ,const QString& fileName
        ,QObject* parent = nullptr
    ) const;


    /*!
     * Writes the given block and all its descendants to their respective block
     * files in the given directory.
     * 
     * Descendant block files are located in their respective subdirectories.
     * 
     * A file system, logical block, or write block exception is thrown if any
     * error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param dir
     *        The directory.
     */
    private:
    void write(
        const ::Block::Abstract& block
        ,const QDir& dir
    );
};
}


#endif
