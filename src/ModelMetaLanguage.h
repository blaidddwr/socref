#ifndef MODEL_META_LANGUAGE_H
#define MODEL_META_LANGUAGE_H
#include <QObject>
namespace Model {
namespace Meta {




/*!
 * This is a meta model class. It represents the meta information for a language
 * implementation.
 * 
 * This class's properties are name, label, and display icon.
 * 
 * The name property is the machine name.
 * 
 * The label property is the user visible name.
 * 
 * The display icon property is the user visible icon.
 */
class Language:
    public QObject
{
    Q_OBJECT
    const QIcon* _displayIcon;
    const QString _label;
    const QString _name;


    /*!
     * Constructs this new meta model with the given name, label, display icon,
     * and parent.
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
     * @param parent
     *        The parent.
     */
    public:
    Language(
        const QString& name
        ,const QString& label
        ,const QIcon& displayIcon
        ,QObject* parent = nullptr
    );


    public:
    virtual ~Language(
    ) override;


    /*!
     * Getter method.
     *
     * @return
     * This meta model's display icon property.
     */
    public:
    const QIcon& displayIcon(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This meta model's label property.
     */
    public:
    const QString& label(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This meta model's name property.
     */
    public:
    const QString& name(
    ) const;
};
}
}


#endif
