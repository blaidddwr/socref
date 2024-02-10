#ifndef STREAM_BLOCK_H
#define STREAM_BLOCK_H
#include <QObject>
#include "Block.h"
#include "Language.h"
class QDir;
class QXmlStreamReader;
class QXmlStreamWriter;
namespace Stream {




/*!
 * This is a stream class. It is a singleton class. It handles all input and
 * output for abstract block class instances.
 */
class Block:
    public QObject
{
    Q_OBJECT;
    static Block* _instance;


    /*!
     * Reads in a root block and all its descendants from a multi block file
     * directory at the given path using the given language and format version.
     * The root block's parent is set to the given parent.
     * 
     * The given language and format version must be valid.
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
     *
     * @return
     * The root block.
     */
    public:
    static ::Block::Abstract* fromDir(
        Language::Abstract* language
        ,int version
        ,const QString& path
        ,QObject* parent = nullptr
    );


    /*!
     * Reads in a block and all its descendants from a given XML stream reader
     * using the given language and format version. The root block's parent is
     * set to the given parent.
     * 
     * The given language and format version must be valid.
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
     * @param xml
     *        The XML stream reader.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The read in block.
     */
    public:
    static ::Block::Abstract* fromXml(
        Language::Abstract* language
        ,int version
        ,QXmlStreamReader& xml
        ,QObject* parent = nullptr
    );


    /*!
     * Returns a list of orphaned block files not used by the given block and
     * its descendants in the multi block file directory at the given path.
     * 
     * A file system or logical block exception is thrown if any error is
     * encountered.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     */
    public:
    static QStringList orphanFiles(
        const ::Block::Abstract& block
        ,const QString& path
    );


    /*!
     * Removes orphaned block files at the given file paths.
     * 
     * All given file paths are verified to be an orphaned block file for the
     * given block at the given directory path.
     * 
     * If the git flag is true then the git command is used for removing
     * deprecated block files, else the standard system remove command is used.
     * 
     * A file system, run system, or logical block exception is thrown if any
     * error is encountered.
     *
     * @param filePaths
     *        The file paths.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The directory path.
     *
     * @param git
     *        The git flag.
     */
    public:
    static void removeOrphanFiles(
        const QStringList& filePaths
        ,const ::Block::Abstract& block
        ,const QString& path
        ,bool git = false
    );


    /*!
     * Writes out the given block and all its descendants to a multi block file
     * directory at the given path.
     * 
     * The given block must be a root block of its project.
     * 
     * A write block, logical block, or file system exception is thrown if any
     * error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     */
    public:
    static void toDir(
        ::Block::Abstract& block
        ,const QString& path
    );


    /*!
     * Writes out the given block and all its descendants to a given XML stream
     * writer.
     * 
     * A write block exception is thrown if any error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param xml
     *        The XML stream writer.
     */
    public:
    static void toXml(
        const ::Block::Abstract& block
        ,QXmlStreamWriter& xml
    );


    private:
    Block(
    ) = delete;


    /*!
     * Inserts the given block's file path and all its descendant's file paths
     * to the given path registry.
     * 
     * All paths have the given path added to them as their root path.
     * 
     * A logical block exception is thrown if any error is encountered.
     *
     * @param registry
     *        The path registry.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     */
    private:
    static void insertBlockPaths(
        QSet<QString>& registry
        ,const ::Block::Abstract& block
        ,const QString& path
    );


    /*!
     * Inserts all paths of found block file paths into the given path list in
     * the given directory path. The given directory path is recursively
     * searched for all children block directories.
     *
     * @param paths
     *        The path list.
     *
     * @param path
     *        The directory path.
     */
    private:
    static void insertPaths(
        QStringList& paths
        ,const QString& path
    );


    /*!
     * Creates and returns a new block and all its children located at the given
     * directory with the given block file name, language, format version, and
     * parent. The given language must be valid.
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
     * @param dir
     *        The directory.
     *
     * @param fileName
     *        The block file name.
     *
     * @param parent
     *        The parent.
     */
    private:
    static ::Block::Abstract* read(
        Language::Abstract* language
        ,int version
        ,const QDir& dir
        ,const QString& fileName
        ,QObject* parent = nullptr
    );


    /*!
     * Writes the block file from the given block to the given directory.
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
    static void write(
        const ::Block::Abstract& block
        ,const QDir& dir
    );
};
}


#endif
