#ifndef CPP_BLOCK_H
#define CPP_BLOCK_H
namespace Cpp {


/*!
 * This contains all C++ blocks.
 */
namespace Block
{
    class Class;
    class Enumeration;
    class EnumerationValue;
    class Exception;
    class Function;
    class Namespace;
    class Property;
    class Union;
    class Variable;


    /*!
     * This enumerates all C++ block implementations' indexes.
     */
    enum Index
    {
        ClassIndex
        ,EnumerationIndex
        ,EnumerationValueIndex
        ,ExceptionIndex
        ,FunctionIndex
        ,NamespaceIndex
        ,PropertyIndex
        ,UnionIndex
        ,VariableIndex
    };
}
}


#endif
