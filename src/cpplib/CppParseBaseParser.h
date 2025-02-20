#ifndef CPP_PARSE_BASEPARSER_H
#define CPP_PARSE_BASEPARSER_H
#include "AbstractParser.h"
namespace Cpp {
namespace Parse {




/*!
 * This extends but does not implement the abstract parser class. It provides
 * common methods used by multiple parser implementations.
 */
class BaseParser:
    public AbstractParser
{


    public:
    BaseParser(
        AbstractParser* parent = nullptr
        ,AbstractBlock* block = nullptr
        ,int version = -1
    );


    /*!
     * Inserts lines of code into this object's block object's code property.
     * 
     * @exception Exception::LogicalParse Thrown when a logical parse error is
     * encountered.
     *
     * @param keyIndex
     *        The C++ code key index. This is used to get the string key used to
     *        insert into the code property. See the C++ code key function for
     *        more documentation.
     *
     * @param lines
     *        The inserted code lines.
     */
    protected:
    void insertCode(
        int keyIndex
        ,const QStringList& lines
    );
};
}
}


#endif
