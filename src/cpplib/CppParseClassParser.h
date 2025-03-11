#ifndef CPP_PARSE_CLASSPARSER_H
#define CPP_PARSE_CLASSPARSER_H
#include <QHash>
#include "AbstractParser.h"
#include "CppBlock.h"
namespace Cpp {
namespace Parse {




/*!
 * This parses the source code of C++ class declarations.
 * 
 * This class uses a hash table for any possible classes it can parse based off
 * its block object. The name of a class is used as the key for the lookup table
 * and it is generated on construction of objects of this class.
 */
class ClassParser:
    public AbstractParser
{
    Q_OBJECT
    using Class = Block::Class;
    Class* _class {nullptr};
    QHash<QString,Class*> _classes;
    int _size;
    int _start;
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
     * Finds the class in this parser object's internal lookup table, using this
     * object's currently parsed declaration parameters, and assigning it to
     * this object's current class pointer.
     * 
     * @exception Exception::LogicalParse
     *
     * @param name
     *        The name of the class which is found.
     */
    private:
    void find(
        const QString& name
    );


    /*!
     * Parses a legacy declaration. See the parse interface for more
     * documentation.
     * 
     * @exception Exception::LogicalParse
     */
    private:
    Status parseLegacy(
        const QStringList& lines
        ,int where
    );


    /*!
     * Parses a version 1 declaration. See the parse interface for more
     * documentation.
     * 
     * @exception Exception::LogicalParse
     */
    private:
    Status parseVersion1(
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
     * Resets this parser object, making it ready to scan for a new class
     * declarations.
     */
    private:
    void reset(
    );
};
}
}


#endif
