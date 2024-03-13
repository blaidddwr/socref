#ifndef TEST_H
#define TEST_H
#include <QString>
class QIcon;
struct TestObject
{
    QString name;
    QObject* ptr;
};


/*!
 * This contains all tests.
 */
namespace Test
{
    class UnitModel;


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
     * Executes a Qt unit test class for this application with the given name.
     * 
     * If the given name is not a valid unit test then a help message is printed
     * with all available tests.
     * 
     * This must be called after the extract arguments function is called.
     *
     * @param name
     *        The name.
     *
     * @return
     * The status of unit test execution.
     */
    int execute(
        const QString& name
    );


    /*!
     * Executes all Qt unit test classes by calling this application with the
     * test option for each possible test.
     * 
     * If the returned status of a test execution is not 0 then this halts test
     * execution and returns the non-zero status.
     * 
     * This must be called after the extract arguments function is called.
     *
     * @return
     * The status of all executed unit tests.
     */
    int executeAll(
    );


    /*!
     * Extracts all command line arguments that will be passed onto the Qt unit
     * test system from the given argument count and values.
     * 
     * This is done by looking for an argument with the special value ":". All
     * arguments after the first argument with the special value is removed from
     * the given command line arguments and saved for passing onto the Qt unit
     * testing system when calling execute or execute all. The special value
     * argument is removed but not passed along.
     * 
     * The arguments are removed by decreasing the given argument count.
     * 
     * This must be called once and only once before the execute or execute all
     * functions are called.
     *
     * @param argc
     *        The argument count.
     *
     * @param argv
     *        The argument values.
     */
    void extractArguments(
        int& argc
        ,char** argv
    );


    /*!
     * Prints all valid unit test names to standard output.
     */
    void listTests(
    );


    /*!
     * Returns all valid unit tests for this application.
     */
    const QList<TestObject>& testObjects(
    );
}


#endif
