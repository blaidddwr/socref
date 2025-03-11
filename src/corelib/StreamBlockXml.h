#ifndef STREAM_BLOCKXML_H
#define STREAM_BLOCKXML_H
#include "Global.h"
class QObject;
class QXmlStreamReader;
class QXmlStreamWriter;
namespace Stream {




/*!
 * This is a stream class. It handles all input and output of XML for abstract
 * block class instances.
 * 
 * Its properties are XML writer, XML reader, language, and version. The XML
 * reader or writer are used as the reader or writer of the XML, respectively,
 * where all loading and saving is done. The language and version are used when
 * loading blocks.
 */
class BlockXml
{
    AbstractLanguage* _language {nullptr};
    QXmlStreamReader* _xmlReader {nullptr};
    QXmlStreamWriter* _xmlWriter {nullptr};
    int _version {-1};


    /*!
     * Constructs this new stream with the given XML writer.
     *
     * @param xml
     *        The XML writer.
     */
    public:
    BlockXml(
        QXmlStreamWriter& xml
    );


    /*!
     * Constructs this new stream with the given XML reader, language, and
     * version.
     * 
     * The given language and version must be valid.
     *
     * @param xml
     *        The XML reader.
     *
     * @param language
     *        The language.
     *
     * @param version
     *        The version.
     */
    public:
    BlockXml(
        QXmlStreamReader& xml
        ,AbstractLanguage* language
        ,int version
    );


    /*!
     * A write block exception is thrown if any error is encountered.
     */
    public:
    BlockXml& operator<<(
        const AbstractBlock& block
    );


    /*!
     * Loads a block with the given parent and all its descendants.
     * 
     * This stream must have been constructed with an XML reader, language, and
     * version.
     * 
     * A read block or file system exception is thrown if any error is
     * encountered.
     *
     * @param parent
     *        The parent.
     *
     * @return
     * The loaded block.
     */
    public:
    AbstractBlock* load(
        QObject* parent = nullptr
    ) const;


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
    private:
    void write(
        const AbstractBlock& block
        ,QXmlStreamWriter& xml
    );
};
}


#endif
