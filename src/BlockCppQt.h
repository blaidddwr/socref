#ifndef BLOCK_CPPQT_H
#define BLOCK_CPPQT_H
#include "BlockCpp.h"
namespace Block {


/*!
 * This contains all C++/Qt block enumerations and classes.
 */
namespace CppQt
{
    class Function;


    /*!
     * Enumerates additional function flags for Qt methods.
     */
    enum FunctionFlags
    {
        QtInvokableFunctionFlag = Cpp::UserFunctionFlag
    };


    /*!
     * Enumerates additional function types for Qt methods.
     */
    enum FunctionType
    {
        SignalFunctionType = Cpp::UserFunctionType
        ,SlotFunctionType
    };


    /*!
     * Enumerates all C++/Qt block implementations' indexes.
     */
    enum Index
    {
        ClassIndex
        ,EnumerationIndex
        ,FunctionIndex
        ,NamespaceIndex
    };
}
}


#endif
