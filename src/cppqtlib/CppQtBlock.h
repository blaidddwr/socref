#ifndef CPPQT_BLOCK_H
#define CPPQT_BLOCK_H
#include "CppBlock.h"
namespace CppQt {


/*!
 * Detailed description.
 */
namespace Block
{
    class Function;


    /*!
     * This enumerates additional function flags for Qt methods.
     */
    enum FunctionFlags
    {
        QtInvokableFunctionFlag = Cpp::Block::UserFunctionFlag
    };


    /*!
     * This enumerates additional function types for Qt methods.
     */
    enum FunctionType
    {
        SignalFunctionType = Cpp::Block::UserFunctionType
        ,SlotFunctionType
    };


    /*!
     * This enumerates all C++/Qt block implementations' indexes.
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
