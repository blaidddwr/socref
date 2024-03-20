#ifndef SETTINGS_SPELLING_H
#define SETTINGS_SPELLING_H
#include <QObject>
class QDir;
namespace Settings {




/*!
 * This is a singleton settings class. It holds all settings pertaining to spell
 * checking in this application using the Hunspell library.
 * 
 * Its properties are path and dictionary.
 * 
 * The path is the directory path where Hunspell dictionaries can be found. The
 * default value is "/usr/share/hunspell".
 * 
 * The dictionary is the name of the dictionary used for spell checking,
 * excluding the extensions used in the dictionary's filenames. A dictionary
 * value of null means no dictionary is selected and spell checking is disabled.
 * The default value is "en_US".
 */
class Spelling:
    public QObject
{
    Q_OBJECT
    QString _dictionary;
    QString _path;
    static Spelling* _instance;


    /*!
     * Signals this settings' dictionary property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void dictionaryChanged(
        const QString& value
    );


    /*!
     * Signals this settings' path property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void pathChanged(
        const QString& value
    );


    /*!
     * Returns the absolute file path of this settings' dictionary's affix file
     * using its path and dictionary properties.
     * 
     * If this setting's dictionary property is null then this returns null.
     *
     * @return
     * The absolute file path.
     */
    public:
    QString absoluteAffPath(
    ) const;


    /*!
     * Returns the absolute file path of this settings' dictionary's dictionary
     * file using its path and dictionary properties.
     * 
     * If this setting's dictionary property is null then this returns null.
     *
     * @return
     * The absolute file path.
     */
    public:
    QString absoluteDicPath(
    ) const;


    /*!
     * Returns a list of valid and readable dictionaries for the given
     * directory.
     * 
     * If the given directory does not exist or is not readable then this
     * returns an empty list.
     *
     * @param dir
     *        The directory.
     *
     * @return
     * The list of valid and readable dictionaries.
     */
    public:
    static QStringList dictionaries(
        const QDir& dir
    );


    /*!
     * Getter method.
     *
     * @return
     * This settings' dictionary property.
     */
    public:
    const QString& dictionary(
    ) const;


    /*!
     * Determines if this settings' has a valid dictionary.
     *
     * @return
     * True if it has a valid dictionary or false otherwise.
     */
    public:
    bool hasDictionary(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The singleton instance of this class.
     */
    public:
    static Spelling* instance(
    );


    /*!
     * Getter method.
     *
     * @return
     * This settings' path property.
     */
    public:
    const QString& path(
    ) const;


    /*!
     * Sets this settings' dictionary property to the given value.
     * 
     * The given value must be null or a valid and readable dictionary contained
     * in the directory at this settings' path property.
     *
     * @param value
     *        The value.
     */
    public:
    void setDictionary(
        const QString& value
    );


    /*!
     * Sets this settings' path property to the given value and its dictionary
     * property to null.
     * 
     * The given value must be a path to a directory which exists and is
     * readable.
     *
     * @param value
     *        The value.
     */
    public:
    void setPath(
        const QString& value
    );


    private:
    Spelling(
        QObject* parent = nullptr
    );


    /*!
     * Determines if the given dictionary is a valid and readable dictionary
     * located in the given directory.
     *
     * @param dir
     *        The directory.
     *
     * @param dictionary
     *        The dictionary.
     *
     * @return
     * True if it is valid and readable or false otherwise.
     */
    private:
    static bool isReadable(
        const QDir& dir
        ,const QString& dictionary
    );
};
}


#endif
