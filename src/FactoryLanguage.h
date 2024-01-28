#ifndef FACTORY_LANGUAGE_H
#define FACTORY_LANGUAGE_H
#include <QObject>
#include <QHash>
#include "Language.h"
#include "ModelMeta.h"
namespace Factory {




/*!
 * This is a factory class. It is a singleton class. It creates abstract
 * language implementations.
 */
class Language:
    public QObject
{
    Q_OBJECT
    QHash<QString,int> _lookup;
    QList<::Language::Abstract*> _languages;
    QList<bool> _isHiddenList;
    static Language* _instance;


    /*!
     * Returns this factory's language with the given index and parent. The
     * given index must be valid.
     *
     * @param index
     *        The index.
     */
    public:
    ::Language::Abstract* get(
        int index
    ) const;


    /*!
     * Returns this factory's language index with the given meta name. If there
     * is no such language with the given meta name then -1 is returned.
     *
     * @param name
     *        The name.
     */
    public:
    int indexFromName(
        const QString& name
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The singleton instance of this class.
     */
    public:
    static Factory::Language* instance(
    );


    /*!
     * Determines if this factory's language with the given index is hidden or
     * visible. The given index must be valid.
     *
     * @param index
     *        The index.
     *
     * @return
     * True if it is hidden else visible.
     */
    public:
    bool isHidden(
        int index
    ) const;


    /*!
     * Returns this factory's language's meta at the given index. The given
     * index must be valid.
     *
     * @param index
     *        The index.
     */
    public:
    Model::Meta::Language* meta(
        int index
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This factory's total number of language implementations.
     */
    public:
    int size(
    ) const;


    private:
    Language(
    );


    /*!
     * Appends the given language to this factory's list of languages. The given
     * language must be valid and its meta name must be unique among all other
     * language meta names in this factory. This factory takes ownership of the
     * given language.
     * 
     * The "is hidden" flag is used to determine if the added language should be
     * visible to the user or not. If the flag is true then it is hidden from
     * the user else it is visible.
     *
     * @param language
     *        The language.
     *
     * @param isHidden
     *        The "is hidden" flag.
     */
    private:
    void appendLanguage(
        ::Language::Abstract* language
        ,bool isHidden = false
    );
};
}


#endif
