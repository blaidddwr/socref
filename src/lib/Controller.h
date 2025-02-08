#ifndef CONTROLLER_H
#define CONTROLLER_H


/*!
 * This contains all controllers.
 */
namespace Controller
{
    class Code;
    class SpellingHighlight;


    /*!
     * This enumerates the legacy version used by all language implementations.
     */
    enum CodeVersion
    {
        Code_Legacy = 0
    };


    /*!
     * This enumerates all versions for C++ source code parsing and building.
     */
    enum CppCodeVersion
    {
        Cpp_Code_Legacy = Code_Legacy
        ,Cpp_Code_1 = 1
    };
}


#endif
