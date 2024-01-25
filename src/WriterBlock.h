#ifndef WRITER_BLOCK_H
#define WRITER_BLOCK_H
#include <QSet>
#include <QObject>
#include "Block.h"
class QDir;
namespace Writer {




/*!
 * This is a writer class. It writes a given block's data and its descendant
 * block's data to a directory at a given directory path using the multi block
 * file system.
 */
class Block
{
    QString _path;
    QSet<QString> _scopes;
    bool _open {false};
    QString tr(const char* text) { return QObject::tr(text); }


    /*!
     * Constructs this new instance with the given directory path.
     *
     * @param path
     *        The directory path.
     */
    public:
    Block(
        const QString& path
    );


    /*!
     * A file system, logical block, or write block exception is thrown if any
     * error is encountered.
     */
    public:
    Writer::Block& operator<<(
        const ::Block::Abstract& block
    );


    /*!
     * Opens this instance's directory path for writing block data files.
     * 
     * A file system exception is thrown if any error is encountered.
     */
    public:
    void open(
    );
};
}


#endif
