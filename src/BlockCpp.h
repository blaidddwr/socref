#ifndef BLOCK_CPP_H
#define BLOCK_CPP_H
namespace Block {


/*!
 * This contains all C++ blocks.
 */
namespace Cpp
{
    class Base;
    class Class;
    class Enumeration;
    class ExceptionItem;
    class Function;
    class Namespace;
    class Property;
    class Union;
    class Variable;


    /*!
     * Enumerates all access types for C++ blocks whose parent is a class block.
     * In order to expand this enumeration one must use the user access as the
     * first new enumeration.
     */
    enum Access
    {
        PublicAccess
        ,ProtectedAccess
        ,PrivateAccess
        ,UserAccess
    };


    /*!
     * Enumerates all function assignments for C++ function blocks. In order to
     * expand this enumeration one must use the user function assignment as the
     * first new enumeration.
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
     * Enumerates all function flags for C++ function blocks. Each flag takes
     * one bit of an integer. In order to expand this enumeration one must use
     * unused bits to the left starting with the user function flag bit.
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
     * Enumerates all function types for C++ function blocks. In order to expand
     * this enumeration one must use the user function type as the first new
     * enumeration.
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
     * Enumerates all C++ block implementations' indexes.
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
