#ifndef BLOCK_CPPQT_H
#define BLOCK_CPPQT_H
#include "BlockCpp.h"
namespace Block {


/*!
 * Contains all C++/Qt block enumerations and classes.
 */
namespace CppQt
{


    /*!
     * Enumerates additional access types for Qt signals and slots.
     */
    enum Access
    {
        SignalAccess = Cpp::UserAccess
        ,PublicSlotAccess
        ,ProtectedSlotAccess
        ,PrivateSlotAccess
    };


    /*!
     * Enumerates additional function flags for Qt methods.
     */
    enum FunctionFlags
    {
        QtInvokableFunctionFlag = Cpp::UserFunctionFlag
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
