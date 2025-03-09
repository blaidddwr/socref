#ifndef CPP_PARSE_FUNCTIONPARSER_H
#define CPP_PARSE_FUNCTIONPARSER_H
#include <QHash>
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of C++ function definitions.
 * 
 * This class uses a hash table for any possible functions it can parse based
 * off its block object. The signature of a function is used as the key for the
 * lookup table and it is generated on construction of objects of this class.
 * The signature includes the scope of its class name of it is a method.
 */
class FunctionParser:
    public AbstractParser
{
    Q_OBJECT
    using Function = Block::Function;
    Function* _function {nullptr};
    QHash<QString,Function*> _functions;
    QString _name;
    QString _scope;
    QStringList _arguments;
    bool _isConstant {false};
    int _size;
    int _start;


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
     * Parses a given string into a list of simplified type only arguments. The
     * names and all extraneous whitespace are removed, allowing the returned
     * list to be used to generate a signature.
     * 
     * @exception Exception::LogicalParse
     *
     * @param arguments
     *        The string containing one or more arguments.
     *
     * @return
     * The list of type only arguments.
     */
    private:
    static QStringList detangle(
        const QString& arguments
    );


    /*!
     * Finds the function in this parser object's internal lookup table, using
     * this object's currently parsed declaration parameters, and assigning it
     * to this object's current internal function.
     * 
     * @exception Exception::LogicalParse
     */
    private:
    void find(
    );


    /*!
     * Parses a legacy or version 1 declaration. See the parse interface for
     * more documentation.
     * 
     * @exception Exception::LogicalParse
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );


    /*!
     * Recursively populates this parser object's internal function lookup
     * table. Any descendant namespace blocks and their descendant functions are
     * ignored.
     * 
     * @exception Exception::LogicalParse
     *
     * @param parent
     *        The parent whose descendant function blocks are added to this
     *        parser object's internal lookup table.
     *
     * @param scope
     *        The class scope used as the scope of each descendant function's
     *        signature. This must be empty if the given parent is a namespace,
     *        otherwise it must contain the descendant method's class name
     *        appended with 2 colon characters.
     */
    private:
    void populate(
        AbstractBlock* parent
        ,const QString& scope
    );


    /*!
     * Resets this parser object, making it ready to scan for a new function
     * definition.
     */
    private:
    void reset(
    );


    /*!
     * Removes the name and all extraneous whitespace of a given argument.
     * 
     * @exception Exception::LogicalParse
     *
     * @param argument
     *        The argument. This cannot be an empty string.
     *
     * @return
     * The type only argument with no extraneous whitespace.
     */
    private:
    static QString toType(
        const QString& argument
    );
};
}
}


#endif
