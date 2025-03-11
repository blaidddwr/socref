#ifndef CPP_PARSE_UNIONPARSER_H
#define CPP_PARSE_UNIONPARSER_H
#include <QHash>
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of C++ union declarations.
 */
class UnionParser:
    public AbstractParser
{
    Q_OBJECT
    using Union = Block::Union;
    Union* _union {nullptr};
    QHash<QString,Union*> _unions;
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
        ,Scanning
    };
    private:
    State _state {State::Scanning};


    public:
    UnionParser(
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
     * Finds a union in this parser object's internal lookup table. The found
     * union is saved to this parser object's internal union pointer.
     * 
     * @exception Exception::LogicalParse
     *
     * @param name
     *        The name of the union which is found.
     */
    private:
    void find(
        const QString& name
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
     * Populates this parser object's internal class lookup table from its block
     * object. Any descendant namespace blocks and their descendant classes are
     * ignored.
     * 
     * @exception Exception::LogicalParse
     */
    private:
    void populate(
    );


    /*!
     * Resets this parser object, making it ready to scan for new union
     * declarations.
     */
    private:
    void reset(
    );
};
}
}


#endif
