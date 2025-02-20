#ifndef CPP_PARSE_H
#define CPP_PARSE_H
namespace Cpp {


/*!
 * This contains all C++ parsers.
 */
namespace Parse
{
    class BaseParser;
    class HeadParser;
    class SourceParser;


    /*!
     * This enumerates all C++ parser indexes.
     */
    enum Index
    {
        HeadParserIndex
        ,SourceParserIndex
    };
}
}


#endif
