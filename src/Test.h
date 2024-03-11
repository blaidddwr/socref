#ifndef TEST_H
#define TEST_H
#include <QtGlobal>
class QIcon;


/*!
 * This contains all unit tests.
 */
namespace Test
{


    /*!
     * Determines if the given first and second Qt icons are equal. This is done
     * by writing them to a Qt byte array and seeing if the arrays are equal.
     *
     * @param icon0
     *        The first icon.
     *
     * @param icon1
     *        The second icon.
     *
     * @return
     * True if they are equal or false otherwise.
     */
    bool areIconsEqual(
        const QIcon& icon0
        ,const QIcon& icon1
    );


    /*!
     * Executes a Qt unit test class for this application, using the given
     * argument count and arguments with the Qt unit test system, excluding the
     * first two.
     * 
     * The first argument is assumed to be "--test". The second argument is the
     * name of the unit test. If the name of the unit test is not valid or not
     * given then a help message is printed with all available tests. All other
     * arguments are passed to the Qt unit test system.
     * 
     * This must be called after the initialize function is called.
     *
     * @param argc
     *        The argument count.
     *
     * @param argv
     *        The arguments.
     *
     * @return
     * The status of unit test execution.
     */
    int execute(
        int argc
        ,char** argv
    );


    /*!
     * Executes all Qt unit test classes by calling this application with the
     * test option for each possible test. If the returned status of a test
     * execution is not 0 then this halts test execution and returns the
     * non-zero status.
     * 
     * The given argument count and arguments are passed along to each
     * individual execution of this application, excluding the first test all
     * option.
     * 
     * This must be called after the initialize function is called.
     *
     * @param argc
     *        The argument count.
     *
     * @param argv
     *        The arguments.
     *
     * @return
     * The status of all executed unit tests.
     */
    int executeAll(
        int argc
        ,char** argv
    );


    /*!
     * Initializes this namespace's internal array of available tests. This must
     * be called before the execute or execute all functions are called.
     */
    void initialize(
    );
}


#endif
