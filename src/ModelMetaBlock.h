#ifndef MODEL_META_BLOCK_H
#define MODEL_META_BLOCK_H
#include "ModelMetaLanguage.h"
namespace Model {
namespace Meta {




/*!
 * This is a meta model class. It represents the meta information for a block
 * implementation.
 * 
 * This class's property are language, index, and allow list.
 * 
 * The language property is the meta information a meta block's language
 * implementation.
 * 
 * The index property is the index used for creating a meta's block from its
 * language.
 * 
 * The allow list property is a set of block indexes that a meta's block is
 * allowed to have as children blocks.
 */
class Block:
    public Language
{
    Q_OBJECT
    Language* _language;
    QSet<int>* _allowList;
    int _index;


    /*!
     * Constructs this new model with the given language, index, name, label,
     * display icon, allow list, and parent.
     * 
     * The given language must be valid and cannot be destroyed during the life
     * of this model.
     *
     * @param language
     *        The language.
     *
     * @param index
     *        The index.
     *
     * @param name
     *        The name.
     *
     * @param label
     *        The label.
     *
     * @param displayIcon
     *        The display icon.
     *
     * @param allowList
     *        The allow list.
     *
     * @param parent
     *        The parent.
     */
    public:
    Block(
        Language* language
        ,int index
        ,const QString& name
        ,const QString& label
        ,const QIcon& displayIcon
        ,const QSet<int>& allowList
        ,QObject* parent = nullptr
    );


    public:
    virtual ~Block(
    ) override;


    /*!
     * Getter method.
     *
     * @return
     * This block's allow list property.
     */
    public:
    const QSet<int>& allowList(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This block's index property.
     */
    public:
    int index(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This model's language property.
     */
    public:
    Language* language(
    ) const;


    /*!
     * Called when this model's language's destroyed signal is emitted.
     */
    private slots:
    void onLanguageDestroyed(
        QObject* object
    );
};
}
}


#endif
