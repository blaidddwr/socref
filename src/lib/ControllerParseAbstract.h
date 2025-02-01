#ifndef CONTROLLER_PARSE_ABSTRACT_H
#define CONTROLLER_PARSE_ABSTRACT_H
#include <QObject>
namespace Controller {
namespace Parse {




/*!
 * This is an abstract class. It parses a source code file for code lines. These
 * code lines in turn are used by builders to rebuild the source code with the
 * implementation code intact.
 * 
 * A parse can have children parses. Children parses look for their own code
 * blocks within the source code. A parse implementation is responsible for
 * adding its children parses at the correct time.
 * 
 * Parsing is done line by line, and a parse is treated like a state machine
 * with its parse interface being called per line of source code. A status is
 * returned instructing the code controller how to proceed. When the children
 * parses of a parse is given a change to parse a line of code, the first child
 * that successfully parses one or more lines is used and all other children are
 * ignored for that pass until the children parses are called to parse a line of
 * code again.
 * 
 * children: This property holds the list of a parse's children.
 * 
 * version: This property holds the source code version a parse is parsing.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    QList<Abstract*> _children;
    int _version {-1};


    /*!
     * This enumerates the status of this parse after parsing a given line.
     * 
     * Ok - This parse parsed the given line and its children will be skipped.
     * 
     * Children - This parse did not parse the given line and its children will
     * be given a chance to parse it.
     * 
     * Done - This parse is done parsing the source code, returning control to
     * its parent or ending parsing of the file if there is no parent parse.
     */
    public:
    enum class Status
    {
        Ok
        ,Children
        ,Done
    };


    /*!
     * Constructs a new parser.
     *
     * @param parent
     *        The new parser's parent.
     */
    public:
    Abstract(
        Abstract* parent = nullptr
    );


    /*!
     * Getter method.
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
     * The status of this parse after parsing the given line of code.
     */
    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) = 0;


    /*!
     * Setter method.
     */
    public:
    void setVersion(
        int value
    );


    /*!
     * Getter method.
     */
    public:
    int version(
    ) const;


    /*!
     * Adds a new child to this parse.
     *
     * @param child
     *        The child parse added to this parse. This parser takes ownership
     *        of the given child.
     */
    protected:
    void addChild(
        Abstract* child
    );
};
}
}


#endif
