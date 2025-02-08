#ifndef CONTROLLER_PARSE_ABSTRACT_H
#define CONTROLLER_PARSE_ABSTRACT_H
#include <QObject>
#include "Block.h"
namespace Controller {
namespace Parse {




/*!
 * This is an abstract class. It parses a source code file for code lines. These
 * code lines in turn are used by builders to rebuild the source code with the
 * implementation code intact.
 * 
 * A parser can have children parsers. Children parsers look for their own code
 * blocks within the source code. A parser implementation is responsible for
 * adding its children parsers at the correct time.
 * 
 * Parsing is done line by line, and a parser is treated like a state machine
 * with its parse interface being called per line of source code. A status is
 * returned instructing the code controller how to proceed. When the children
 * parsers of a parser are given a change to parse a line of code, the first
 * child that successfully parses one or more lines is used and all other
 * children are ignored for that pass until the children parsers are called to
 * parse a line of code again.
 * 
 * block: This property holds the block associated with the source code a parser
 * is parsing. This must be a valid block before parsing can begin.
 * 
 * children: This property holds the list of a parser's children.
 * 
 * version: This property holds the version of source code a parser is parsing.
 * This must be a valid version number before parsing can begin.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    Block::Abstract* _block {nullptr};
    QList<Abstract*> _children;
    int _version {-1};


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
     * @param parent
     *        The new parser's parent. If this is a root parser created by the
     *        language's create parser interface then this must be null, else it
     *        must be a valid parser.
     */
    public:
    Abstract(
        Abstract* parent = nullptr
    );


    /*!
     * Getter for children property.
     */
    public:
    const QList<Abstract*>& children(
    );


    /*!
     * Parses a single line of code.
     *
     * @param lines
     *        The lines of source code.
     *
     * @param where
     *        The specific line to be parsed.
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
     * Setter for block property.
     */
    public:
    virtual void setBlock(
        Block::Abstract* object
    );


    /*!
     * Setter for version property.
     */
    public:
    virtual void setVersion(
        int value
    );


    /*!
     * Getter for version property.
     */
    public:
    int version(
    ) const;


    /*!
     * Adds a new child to its parser.
     *
     * @param child
     *        The child parser added. This method's parser takes ownership of
     *        the given child.
     */
    protected:
    void addChild(
        Abstract* child
    );


    /*!
     * Getter for block property.
     * 
     * The given type T must be the specific block implementation of the block
     * property's object.
     */
    protected:
    template<class T> T* block(
    ) const;
};
}
}


#endif
