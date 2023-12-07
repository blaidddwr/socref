#ifndef MODEL_META_LANGUAGE_H
#define MODEL_META_LANGUAGE_H
#include <QObject>
namespace Model {
namespace Meta {




/*!
 * This is a meta model class. It represents the meta information for a language
 * implementation.
 * 
 * Its properties are name and label. The name is the machine name and the label
 * is the user visible name.
 */
class Language:
    public QObject
{
    Q_OBJECT
    const QString _label;
    const QString _name;


    /*!
     * Constructs this new model with the given name, label, and parent.
     *
     * @param name
     *        The name.
     *
     * @param label
     *        The label.
     *
     * @param parent
     *        The parent.
     */
    public:
    Language(
        const QString& name
        ,const QString& label
        ,QObject* parent = nullptr
    );


    /*!
     * Getter method.
     *
     * @return
     * This model's label property.
     */
    public:
    const QString& label(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This model's name property.
     */
    public:
    const QString& name(
    ) const;
};
}
}


#endif
