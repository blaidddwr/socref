#ifndef CPP_PARSE_CLASSPARSER_H
#define CPP_PARSE_CLASSPARSER_H
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of C++ class declarations.
 */
class ClassParser:
    public AbstractParser
{
    Q_OBJECT
    using Class = Block::Class;
    Class* _class;
    int _start;
    int _size;
    static const QRegularExpression _classRe;


    /*!
     * This enumerates all possible states.
     */
    public:
    enum class State
    {
        Body
        ,Declaration
        ,Footer
        ,Header
        ,Scanning
    };
    private:
    State _state {State::Scanning};


    public:
    ClassParser(
        Class* block
        ,int version
        ,QObject* parent = nullptr
    );


    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) override final;


    /*!
     * Parses a legacy declaration. See the parse interface for more
     * documentation.
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );


    /*!
     * Parses a version 1 declaration. See the parse interface for more
     * documentation.
     */
    private:
    Status parseVersion1(
        const QStringList& lines
        ,int where
    );
};
}
}


#endif
