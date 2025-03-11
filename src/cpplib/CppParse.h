#ifndef CPP_PARSE_H
#define CPP_PARSE_H
namespace Cpp {


/*!
 * This contains all C++ parsers.
 */
namespace Parse
{
    class ClassParser;
    class FunctionParser;
    class HeadParser;
    class SourceParser;
    class UnionParser;


    /*!
     * This enumerates the indexes of C++ parsers that must be created by the
     * C++ language class.
     */
    enum Index
    {
        HeadParserIndex
        ,SourceParserIndex
    };
}
}


#endif
