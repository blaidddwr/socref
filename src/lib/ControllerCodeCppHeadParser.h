#ifndef CONTROLLER_CODE_CPP_HEADPARSER_H
#define CONTROLLER_CODE_CPP_HEADPARSER_H
#include "ControllerCodeAbstractParser.h"
namespace Controller {
namespace Code {
namespace Cpp {




/*!
 * This is a C++ parse code controller. It is a root parser responsible for
 * parsing a C++ header file.
 */
class HeadParser:
    public AbstractParser
{
    Q_OBJECT
    QStringList _head;


    /*!
     * This enumerates all possible states of of a head parser.
     * 
     * Body - the parser is reading the body of its header file where its
     * children parsers can find their respective code blocks.
     * 
     * Guard - the parser is reading the very beginning of its header file where
     * the header guard lines are located.
     * 
     * Header - The parser is reading the header lines of its header file.
     */
    public:
    enum class State
    {
        Body
        ,Guard
        ,Header
    };
    State _state {State::Guard};


    public:
    HeadParser(
        AbstractParser* parent = nullptr
    );


    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) override final;


    public:
    virtual void setBlock(
        Block::Abstract* object
    ) override final;


    public:
    virtual void setVersion(
        int value
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
}


#endif
