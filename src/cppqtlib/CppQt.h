#ifndef CPPQT_H
#define CPPQT_H
#include "Cpp.h"


/*!
 * This contains all blocks.
 * 
 * See the abstract block class for a detailed description of blocks.
 */
namespace CppQt
{
    class Language;


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
}


#endif
