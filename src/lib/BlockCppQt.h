#ifndef BLOCK_CPPQT_H
#define BLOCK_CPPQT_H
#include "BlockCpp.h"
namespace Block {


/*!
 * This contains all C++/Qt blocks.
 */
namespace CppQt
{
    class Function;


    /*!
     * This enumerates additional function flags for Qt methods.
     */
    enum FunctionFlags
    {
        QtInvokableFunctionFlag = Cpp::UserFunctionFlag
    };


    /*!
     * This enumerates additional function types for Qt methods.
     */
    enum FunctionType
    {
        SignalFunctionType = Cpp::UserFunctionType
        ,SlotFunctionType
    };


    /*!
     * This enumerates all C++/Qt block implementations' indexes.
     */
    enum Index
    {
        ClassIndex
        ,EnumerationIndex
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
