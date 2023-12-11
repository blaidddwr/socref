#ifndef TEST_BLOCK_ABSTRACT_H
#define TEST_BLOCK_ABSTRACT_H
#include <QObject>
#include "BlockTest.h"
#include "Language.h"
#include "ModelMeta.h"
class QDir;
namespace Test {
namespace Block {




/*!
 * This is a Qt unit test class. It tests the abstract block class.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    ::Block::Test::Node* _block;
    Language::Test* _language;
    Model::Meta::Language* _meta;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the append method.
     */
    private slots:
    void append(
    );


    /*!
     * Tests the descendants method.
     */
    private slots:
    void descendants(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    /*!
     * Tests the display text property.
     */
    private slots:
    void displayTextProperty(
    );


    /*!
     * Tests the from directory method.
     */
    private slots:
    void fromDir(
    );


    /*!
     * Tests the from XML method.
     */
    private slots:
    void fromXml(
    );


    /*!
     * Tests the from XML method using the legacy format.
     */
    private slots:
    void fromXmlLegacy(
    );


    /*!
     * Tests the get method.
     */
    private slots:
    void get(
    );


    /*!
     * Tests the index of method.
     */
    private slots:
    void indexOf(
    );


    /*!
     * Tests the insert method.
     */
    private slots:
    void insert(
    );


    /*!
     * Tests the meta property.
     */
    private slots:
    void metaProperty(
    );


    /*!
     * Tests the scope property.
     */
    private slots:
    void scopeProperty(
    );


    /*!
     * Tests the size property.
     */
    private slots:
    void size(
    );


    /*!
     * Tests the take method.
     */
    private slots:
    void take(
    );


    /*!
     * Tests the to directory method.
     */
    private slots:
    void toDir(
    );


    /*!
     * Tests the to XML method.
     */
    private slots:
    void toXml(
    );


    private slots:
    void cleanupTestCase(
    );


    /*!
     * Determines if the given first and second directories are equal. This is
     * determined if the directories contain the same files and all those files
     * are also equal.
     *
     * @param dir0
     *        The first directory.
     *
     * @param dir1
     *        The second directory.
     *
     * @return
     * True if they are equal or false otherwise.
     */
    private:
    static bool areDirsEqual(
        const QDir& dir0
        ,const QDir& dir1
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
    private:
    static bool areFilesEqual(
        const QString& path0
        ,const QString& path1
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
    private:
    static bool areXmlFilesEqual(
        const QString& path0
        ,const QString& path1
    );


    /*!
     * Creates and returns a new test node block. The returned block's parent is
     * this unit test instance.
     */
    private:
    ::Block::Test::Node* create(
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
     * Creates the test project directory used for unit testing the from and to
     * directory methods. The root unit testing directory for the abstract block
     * test must exist.
     */
    private:
    static void createTestProjectDir(
    );


    /*!
     * Creates the test XML file using for unit testing the latest to and from
     * XML methods. The root unit testing directory for the abstract block test
     * must exist.
     */
    private:
    static void createTestXml(
    );


    /*!
     * Creates the test XML legacy file using for unit testing the latest from
     * XML method using the legacy format. The root unit testing directory for
     * the abstract block test must exist.
     */
    private:
    static void createTestXmlLegacy(
    );
};
}
}


#endif
