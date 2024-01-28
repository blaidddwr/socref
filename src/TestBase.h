#ifndef TEST_BASE_H
#define TEST_BASE_H
#include <QObject>
#include "Language.h"
class QDir;
class QXmlStreamWriter;
namespace Test {




/*!
 * This is a Qt unit test class. It is a base class providing common
 * functionality that many specific unit test classes require.
 */
class Base:
    public QObject
{
    Q_OBJECT
    Language::Abstract* _language {nullptr};


    /*!
     * Determines if the given first and second directories are equal. Only
     * files matching the given filter of each directory are checked for
     * equality, whereas sub directories and unmatched files are ignored.
     *
     * @param dir0
     *        The first directory.
     *
     * @param dir1
     *        The second directory.
     *
     * @param filter
     *        The filter.
     *
     * @return
     * True if they are equal or false otherwise.
     */
    protected:
    static bool areDirsEqual(
        const QDir& dir0
        ,const QDir& dir1
        ,const QString& filter = QString()
    );


    /*!
     * Determines if the first and second files at the given paths are equal. If
     * either file cannot be opened they are considered to be not equal.
     *
     * @param path0
     *        The first path.
     *
     * @param path1
     *        The second path.
     *
     * @return
     * True if they are equal or false otherwise.
     */
    protected:
    static bool areFilesEqual(
        const QString& path0
        ,const QString& path1
    );


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
    protected:
    static bool areIconsEqual(
        const QIcon& icon0
        ,const QIcon& icon1
    );


    /*!
     * Determines if the first and second XML files at the given paths are
     * equal. If either file cannot be opened they are considered to be not
     * equal.
     *
     * @param path0
     *        The first path.
     *
     * @param path1
     *        The second path.
     *
     * @return
     * True if they are equal or false otherwise.
     */
    protected:
    static bool areXmlFilesEqual(
        const QString& path0
        ,const QString& path1
    );


    /*!
     * Cleans up this application's unit testing directory by removing it if it
     * exists.
     */
    protected:
    void cleanup(
    );


    /*!
     * Creates and returns a new block of the given type T with the given block
     * index. The returned block's parent is this unit test instance. This
     * instance's language must be initialized before this is called. The given
     * block index must match the given block type T.
     *
     * @param index
     *        The block index.
     */
    protected:
    template<class T> T* create(
        int index
    );


    /*!
     * Creates this application's unit test directory with all mock up test
     * files. If the unit test directory already exists it is removed before
     * this creates it again.
     */
    protected:
    static bool createTestDir(
    );


    /*!
     * Initializes this instance's language to the given one. The given language
     * must be valid. This must be called before this instance's create method
     * is called. This instance's language can only be initialized once.
     *
     * @param language
     *        The language.
     */
    protected:
    void initLanguage(
        Language::Abstract* language
    );


    /*!
     * Returns this instance's language. This instance's language must be
     * initialized before this is called.
     */
    protected:
    Language::Abstract* language(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The absolute path to this application's unit test directory.
     */
    protected:
    QString testDir(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The absolute path to this application's test multi-file project
     * directory. This can be used for unit testing the loading or saving of
     * project directories.
     */
    protected:
    QString testProjDir(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The absolute path to this application's test exported XML project file.
     * This can be used for unit testing the import and export of projects.
     */
    protected:
    QString testXml(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The absolute path to this application's test exported XML legacy project
     * file. This can be used for unit testing the import and export of projects
     * using the legacy format.
     */
    protected:
    QString testXmlLegacy(
    ) const;


    /*!
     * Writes the end project tags to the given XML writer required in a valid
     * XML project file.
     *
     * @param xml
     *        The XML writer.
     */
    protected:
    static void writeEndProjectXml(
        QXmlStreamWriter& xml
    );


    /*!
     * Writes the start project tags to the given XML writer required in a valid
     * XML project file. The project's name is "Testing123", its language is
     * "test", and its relative parse path is "../testing".
     *
     * @param xml
     *        The XML writer.
     */
    protected:
    static void writeStartProjectXml(
        QXmlStreamWriter& xml
    );


    /*!
     * Creates a test node block file at the given path with the given name and
     * child links. If a file exists at the given path then it is overwritten.
     *
     * @param path
     *        The path.
     *
     * @param name
     *        The name.
     *
     * @param links
     *        The children links.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    static bool createBlockFile(
        const QString& path
        ,const QString& name
        ,const QStringList links
    );


    /*!
     * Creates a test project configuration file for a multi-file project
     * directory at the given path. If a file exists at the given path it is
     * overwritten.
     *
     * @param path
     *        The path.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    static bool createTestProjectConfig(
        const QString& path
    );


    /*!
     * Creates the test multi-file project directory in this application's unit
     * testing directory.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    static bool createTestProjectDir(
    );


    /*!
     * Creates the test XML project file in this applications unit testing
     * directory.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    static bool createTestXml(
    );


    /*!
     * Creates the test XML legacy project file in this applications unit
     * testing directory.
     *
     * @return
     * True on success or false otherwise.
     */
    private:
    static bool createTestXmlLegacy(
    );
};
}


#endif
