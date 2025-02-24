#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H
#include <QObject>
#include "Global.h"




/*!
 * This parses a source code file for code lines. These code lines in turn are
 * used by builders to rebuild the source code with the implementation code
 * intact.
 * 
 * A parser can have children parsers. Children parsers look for their own code
 * blocks within the source code. A parser implementation is responsible for
 * adding its children at construction.
 * 
 * Parsing is done line by line, and a parser is treated like a state machine
 * with its parse interface being called per line of source code. A status is
 * returned instructing the code controller how to proceed. When the children
 * parsers of a parser are given a chance to parse a line of code, the first
 * child that successfully parses one or more lines is used and all other
 * children are ignored for that pass until the children parsers are called to
 * parse a line of code again.
 * 
 * When parsing reaches the end of the source code lines, a special end of lines
 * value is passed as the line to be parsed. The parser must return a status of
 * "done with read" to indicate it was ready for the end of the source code to
 * be reached, otherwise a logical parse error will be thrown. All parent
 * parsers are also given the same end of lines value and must return the "done
 * with read" status to indicate they all expect the source code to end.
 * 
 * @property EOL A special integer value indicating the end of lines has been
 * reached while parsing a source code file's lines.
 * 
 * @property block The block object associated with the source code a parser is
 * parsing. This must be a valid object and it cannot be destroyed during the
 * lifetime of the parser object.
 * 
 * @property version The version of source code a parser is parsing. This must
 * be a valid version number.
 */
class AbstractParser:
    public QObject
{
    Q_OBJECT
    AbstractBlock* _block;
    QList<AbstractParser*> _children;
    int _version;
    public:
    static constexpr int EOL = -1;


    /*!
     * This enumerates the status of a parser after parsing a given line.
     * 
     * DelegateToChildren - The parser did not parse the given line and its
     * children will be given a chance to parse it.
     * 
     * DoneWithRead - The parser is done parsing the source code with the given
     * line being parsed by it.
     * 
     * DoneWithoutRead - The parser is done parsing the source code without the
     * given line being parsed by it.
     * 
     * Read - The parser read the given line and is ready to parse the next
     * line.
     */
    public:
    enum class Status
    {
        DelegateToChildren
        ,DoneWithRead
        ,DoneWithoutRead
        ,Read
    };


    /*!
     * Constructor.
     *
     * @param block
     *        The initial object of the block property.
     *
     * @param version
     *        The initial value of the version property.
     *
     * @param parent
     *        This object's parent. If the parent is an abstract parser then
     *        this object is added to its list of parser children.
     */
    public:
    AbstractParser(
        AbstractBlock* block
        ,int version
        ,QObject* parent = nullptr
    );


    /*!
     * Detailed description.
     */
    public:
    AbstractBlock* block(
    ) const;


    /*!
     * Getter.
     *
     * @return
     * A list of this object's parser children.
     */
    public:
    const QList<AbstractParser*>& children(
    );


    /*!
     * Parses a single line of code. The reset method must be called before this
     * method is called.
     * 
     * @exception Exception::LogicalParse Thrown when a logical parse error is
     * encountered.
     *
     * @param lines
     *        The lines of source code.
     *
     * @param where
     *        The specific line to be parsed or EOL.
     *
     * @return
     * The status of the parser after parsing the given line of code.
     */
    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) = 0;


    /*!
     * Getter for the version property.
     */
    public:
    int version(
    ) const;


    /*!
     * Inserts code lines into this object's block object's code property. This
     * object's block property must have a valid object.
     * 
     * @exception Exception::LogicalParse Thrown when a logical parse error is
     * encountered.
     *
     * @param key
     *        The code map key.
     *
     * @param lines
     *        The inserted code lines. If this is empty then this method does
     *        nothing.
     */
    protected:
    void insertCode(
        const QString& key
        ,const QStringList& lines
    );


    /*!
     * Called when the block object's destroyed signal emitted.
     */
    private slots:
    void onBlockDestroyed(
        QObject* object
    );


    /*!
     * Called when a child parser of this object has its destroyed signal
     * emitted.
     */
    private slots:
    void onChildDestroyed(
        QObject* object
    );
};


#endif
