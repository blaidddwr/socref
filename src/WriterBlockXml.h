#ifndef WRITER_BLOCKXML_H
#define WRITER_BLOCKXML_H
#include "Block.h"
class QXmlStreamWriter;
namespace Writer {




/*!
 * This is a writer class. It writes a given block's data and its descendant
 * block's data to a given XML stream writer.
 */
class BlockXml
{
    QXmlStreamWriter& _xml;


    /*!
     * Constructs this new instance with the given XML stream writer.
     *
     * @param xml
     *        The XML stream writer.
     */
    public:
    BlockXml(
        QXmlStreamWriter& xml
    );


    /*!
     * A write block exception is thrown if any error is encountered.
     */
    public:
    Writer::BlockXml& operator<<(
        const ::Block::Abstract& block
    );
};
}


#endif
