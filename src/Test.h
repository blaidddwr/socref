#ifndef TEST_H
#define TEST_H


/*!
 * Contains all unit test namespaces and functions.
 */
namespace Test
{


    /*!
     * Executes all unit tests for this application, using the given argument
     * count and arguments with the Qt unit test system. The given argument
     * count and arguments must be valid and come from the main function's
     * arguments.
     * 
     * The arguments unique to this program are removed from the argument list.
     * If this is not done the Qt unit test system will fail with unrecognized
     * arguments.
     *
     * @param argc
     *        The argument count.
     *
     * @param argv
     *        The arguments.
     *
     * @return
     * The status of execution.
     */
    int execute(
        int argc
        ,char** argv
    );
}


#endif
