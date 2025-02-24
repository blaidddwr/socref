#ifndef CPP_PARSE_SOURCEPARSER_H
#define CPP_PARSE_SOURCEPARSER_H
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of a C++ source file.
 */
class SourceParser:
    public AbstractParser
{
    Q_OBJECT
    using Class = Block::Class;
    using Namespace = Block::Namespace;
    QStringList _header;
    QStringList _preProcess;


    /*!
     * This enumerates all possible states.
     */
    public:
    enum class State
    {
        Body
        ,Include
        ,Header
        ,Namespace
        ,PreProcess
    };
    private:
    State _state {State::Include};


    public:
    SourceParser(
        Class* block
        ,int version
        ,QObject* parent = nullptr
    );


    public:
    SourceParser(
        Namespace* block
        ,int version
        ,QObject* parent = nullptr
    );


    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) override final;


    /*!
     * Parses a legacy or version 1 header file. See the parse interface for
     * more documentation.
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );
};
}
}


#endif
