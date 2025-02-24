#ifndef CPP_PARSE_HEADPARSER_H
#define CPP_PARSE_HEADPARSER_H
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of a C++ header file.
 */
class HeadParser:
    public AbstractParser
{
    Q_OBJECT
    using Class = Block::Class;
    using Namespace = Block::Namespace;
    bool _childrenAdded {false};
    QStringList _footer;
    QStringList _header;
    QStringList _preProcess;


    /*!
     * This enumerates all possible states.
     */
    public:
    enum class State
    {
        Body
        ,End
        ,Footer
        ,Guard
        ,Header
        ,Namespace
        ,PreProcess
    };
    private:
    State _state {State::Guard};


    public:
    HeadParser(
        Class* block
        ,int version
        ,QObject* parent = nullptr
    );


    public:
    HeadParser(
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
     * Parses a legacy header file. See the parse interface for more
     * documentation.
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );


    /*!
     * Parses a version 1 header file. See the parse interface for more
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
