#ifndef CPP_PARSE_HEADPARSER_H
#define CPP_PARSE_HEADPARSER_H
#include "CppParseBaseParser.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of a C++ header file.
 */
class HeadParser:
    public BaseParser
{
    Q_OBJECT
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
        AbstractParser* parent = nullptr
    );


    public:
    virtual Status parse(
        const QStringList& lines
        ,int where
    ) override final;


    public:
    virtual void reset(
    ) override final;


    public:
    virtual void setBlock(
        AbstractBlock* object
    ) override final;


    public:
    virtual void setVersion(
        int value
    ) override final;


    /*!
     * Adds this object's children parser objects. If this object's children
     * have already been added then this does nothing.
     */
    private:
    void addChildren(
    );


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
