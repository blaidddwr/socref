#ifndef STREAM_BLOCK_H
#define STREAM_BLOCK_H
#include <QObject>
#include "Block.h"
#include "Language.h"
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
     * Returns a list of deprecated block files not used by the given block and
     * its descendants in the multi block file directory at the given path.
     * 
     * A file system exception is thrown if any error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     */
    public:
    static QStringList deprecatedFiles(
        const ::Block::Abstract& block
        ,const QString& path
    );


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
     * Removes deprecated block files not used by the given block and its
     * descendants in the multi block file directory at the given path.
     * 
     * If the git flag is true then the git command is used for removing
     * deprecated block files, else the standard system remove command is used.
     * 
     * A file or run system exception is thrown if any error is encountered.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     *
     * @param git
     *        The git flag.
     */
    public:
    static void removeFiles(
        const ::Block::Abstract& block
        ,const QString& path
        ,bool git = false
    );


    /*!
     * Writes out the given block and all its descendants to a multi block file
     * directory at the given path.
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
        const ::Block::Abstract& block
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
    static ::Block::Abstract* read(
        Language::Abstract* language
        ,int version
        ,const QString& path
        ,QObject* parent = nullptr
    );


    /*!
     * Writes the block file from the given block to the given path.
     * 
     * A file system or write block exception is thrown if any error is
     * encountered.
     *
     * @param block
     *        The block.
     *
     * @param path
     *        The path.
     */
    private:
    static void write(
        const ::Block::Abstract& block
        ,const QString& path
    );
};
}


#endif
