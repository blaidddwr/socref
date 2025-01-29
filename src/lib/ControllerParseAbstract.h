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
 * blocks within the source code, taking precedence over their parent parse. The
 * first line is except and always given to the parent parse.
 * 
 * children: This property holds the list of a parse's children.
 * 
 * childrenIndexes: This property holds the list of a parse's children parse
 * indexes. It is used to create a parse's children parses.
 * 
 * languageIndex: This property holds a parser's language index. It is used to
 * create a parse's children parses.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    QList<Abstract*> _children;
    bool _createdChildren {false};


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
     * Getter method.
     */
    public:
    virtual const QList<int>& childrenIndexes(
    ) const = 0;


    /*!
     * Getter method.
     */
    public:
    virtual int languageIndex(
    ) const = 0;


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
     * True if the line of code was parsed or false otherwise. If the line of
     * code was not parsed it is in turn parsed by the parent parser. If there
     * is no parent parser then the source code finishes parsing.
     */
    public:
    virtual bool parse(
        const QStringList& lines
        ,int where
    ) = 0;
};
}
}


#endif
