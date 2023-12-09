#ifndef BLOCK_CPP_H
#define BLOCK_CPP_H
namespace Block {


/*!
 * This contains all C++ block enumerations and classes.
 */
namespace Cpp
{
    class Base;
    class Class;
    class Enumeration;
    class Namespace;


    /*!
     * Enumerates all access types for C++ blocks whose parent is a class block.
     */
    enum Access
    {
        PublicAccess
        ,ProtectedAccess
        ,PrivateAccess
        ,UserAccess
    };


    /*!
     * Enumerates all function flags for C++ function blocks. Each flag takes
     * one bit of an integer. To expand this enumeration it must use unused bits
     * to the left starting with the user function flag bit.
     */
    enum FunctionFlags
    {
        NoExceptFunctionFlag = 1
        ,DefaultFunctionFlag = 2
        ,DeletedFunctionFlag = 4
        ,ExplicitFunctionFlag = 8
        ,StaticFunctionFlag = 16
        ,ConstantFunctionFlag = 32
        ,VirtualFunctionFlag = 64
        ,OverrideFunctionFlag = 128
        ,FinalFunctionFlag = 256
        ,AbstractFunctionFlag = 512
        ,UserFunctionFlag = 1024
    };


    /*!
     * Enumerates all function types for C++ function blocks.
     */
    enum class FunctionType
    {
        Regular
        ,Method
        ,Constructor
        ,Destructor
        ,Operator
    };


    /*!
     * Enumerates all C++ block implementations' indexes.
     */
    enum Index
    {
        ClassIndex
        ,EnumerationIndex
        ,NamespaceIndex
    };
}
}


#endif
