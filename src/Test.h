#ifndef TEST_H
#define TEST_H
#include <QtGlobal>


/*!
 * Contains all unit test namespaces and functions.
 */
namespace Test
{


    /*!
     * Executes a Qt unit test class for this application, using the given
     * argument count and arguments with the Qt unit test system, excluding the
     * first two.
     * 
     * The first argument is assumed to be "--test". The second argument is the
     * name of the unit test. If the name of the unit test is not valid or not
     * given then a help message is printed with all available tests.
     * 
     * All other arguments are passed to the Qt unit test system.
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
