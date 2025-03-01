#ifndef CPP_PARSE_FUNCTIONPARSER_H
#define CPP_PARSE_FUNCTIONPARSER_H
#include <QHash>
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of C++ function definitions.
 */
class FunctionParser:
    public AbstractParser
{
    Q_OBJECT
    using Function = Block::Function;
    Function* _function {nullptr};
    QHash<QString,Function*> _functions;
    QString _className;
    QString _name;
    QStringList _arguments;
    bool _isConstant {false};
    int _start;
    int _size;


    /*!
     * This enumerates all possible states.
     */
    public:
    enum class State
    {
        Body
        ,Declaration
        ,Header
        ,Middle
        ,Scanning
    };
    private:
    State _state {State::Scanning};


    public:
    FunctionParser(
        AbstractBlock* block
        ,int version
        ,QObject* parent = nullptr
    );


    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) override final;


    /*!
     * Simplifies and removed the name of a given argument. In this context
     * simplification means removing any whitespace at the beginning or end of
     * the argument along with reducing any separating whitespace into a single
     * space.
     *
     * @param argument
     *        The raw argument which is simplified.
     *
     * @return
     * The simplified type only argument.
     */
    private:
    static QString detangleArgument(
        const QString& argument
    );


    /*!
     * Parses a given string into a list of type only arguments. The names are
     * removed so the returned list can be used to generate a signature.
     *
     * @param arguments
     *        The string containing one or more arguments.
     *
     * @return
     * The list of type only arguments.
     */
    private:
    static QStringList detangleArguments(
        const QString& arguments
    );


    /*!
     * Finds the function in this parser object's internal lookup table, using
     * this object's currently parsed declaration parameters, and assigning it
     * to this object's current internal function.
     * 
     * @exception Exception::LogicalParse Thown if a logical parse error is
     * encountered.
     */
    private:
    void findFunction(
    );


    /*!
     * Parses a legacy or version 1 declaration. See the parse interface for
     * more documentation.
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );


    /*!
     * Recursively populates this parser object's internal function lookup
     * table.
     * 
     * @exception Exception::LogicalParse Thown if a logical parse error is
     * encountered.
     *
     * @param parent
     *        The parent whose descendant function blocks are added to this
     *        parser object's internal lookup table.
     *
     * @param className
     *        The class name used as the scope of each descendant function's
     *        signature. This must be empty if the given parent is a namespace,
     *        otherwise it must contain the descendant method's class name.
     */
    private:
    void populateFunctions(
        AbstractBlock* parent
        ,const QString& className
    );


    /*!
     * Resets this parser object, making it ready to scan for a new function
     * definition.
     */
    private:
    void reset(
    );
};
}
}


#endif
