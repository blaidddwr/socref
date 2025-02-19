#ifndef CPP_H
#define CPP_H
#include "Global.h"
class QString;


/*!
 * This contains the C++ language implementation.
 */
namespace Cpp
{
    class Language;
    class Router;


    /*!
     * This enumerates all access types for C++ blocks whose parent is a class
     * block. In order to expand this enumeration one must use the user access
     * as the first new enumeration.
     */
    enum Access
    {
        PublicAccess
        ,ProtectedAccess
        ,PrivateAccess
        ,UserAccess
    };


    /*!
     * This enumerates all possible code keys. See the code key function for
     * further details.
     */
    enum CodeKey
    {
        PreProcessHeadCodeKey
    };


    /*!
     * This enumerates all function assignments for C++ function blocks. In
     * order to expand this enumeration one must use the user function
     * assignment as the first new enumeration.
     */
    enum FunctionAssignment
    {
        NoFunctionAssignment
        ,DefaultFunctionAssignment
        ,DeleteFunctionAssignment
        ,AbstractFunctionAssignment
        ,UserFunctionAssignment
    };


    /*!
     * This enumerates all function flags for C++ function blocks. Each flag
     * takes one bit of an integer. In order to expand this enumeration one must
     * use unused bits to the left starting with the user function flag bit.
     */
    enum FunctionFlags
    {
        NoExceptFunctionFlag = 1
        ,ExplicitFunctionFlag = 2
        ,StaticFunctionFlag = 4
        ,ConstantFunctionFlag = 8
        ,VirtualFunctionFlag = 16
        ,OverrideFunctionFlag = 32
        ,FinalFunctionFlag = 64
        ,UserFunctionFlag = 128
    };


    /*!
     * This enumerates all function types for C++ function blocks. In order to
     * expand this enumeration one must use the user function type as the first
     * new enumeration.
     */
    enum FunctionType
    {
        RegularFunctionType
        ,MethodFunctionType
        ,ConstructorFunctionType
        ,DestructorFunctionType
        ,OperatorFunctionType
        ,UserFunctionType
    };


    /*!
     * This enumerates all source code versions for C++ parsing and building.
     */
    enum Version
    {
        Cpp_Legacy = CODE_LEGACY
        ,Cpp_1 = 1
        ,Cpp_Current = Cpp_1
    };


    /*!
     * Getter for code keys. Code keys are used for storing and retrieving code
     * lines to and from their respective C++ blocks used by parsers and
     * builders, respectively.
     *
     * @param index
     *        The index of the code key returned. This must be a valid code key
     *        index.
     *
     * @return
     * The code key.
     */
    const QString& codeKey(
        int index
    );
}


#endif
