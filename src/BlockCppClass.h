#ifndef BLOCK_CPP_CLASS_H
#define BLOCK_CPP_CLASS_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ class.
 * 
 * Its properties are parents and templates. The parents is a list of a class's
 * parent classes. The templates list is a list of template arguments for a
 * class.
 */
class Class:
    public Block::Cpp::Base
{
    Q_OBJECT
    QStringList _parents;
    QStringList _templates;
    const QIcon* _icon;
    public:
    using ::Block::Cpp::Base::Base;


    /*!
     * Signals this block's parents property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void parentsChanged(
        const QStringList& value
    );


    /*!
     * Signals this block's templates property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void templatesChanged(
        const QStringList& value
    );


    public:
    Class(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    /*!
     * Determines if this class is abstract. The given ignore pointer is used to
     * ignore any matching child function block in determining if this class is
     * abstract.
     *
     * @param ignore
     *        The ignore pointer.
     *
     * @return
     * True if it is abstract otherwise false.
     */
    public:
    bool isAbstract(
        const Block::Abstract* ignore = nullptr
    ) const;


    /*!
     * Determines if this class is virtual. The given ignore pointer is used to
     * ignore any matching child function block in determining if this class is
     * virtual.
     *
     * @param ignore
     *        The ignore pointer.
     *
     * @return
     * True if it is virtual otherwise false.
     */
    public:
    bool isVirtual(
        const Block::Abstract* ignore = nullptr
    ) const;


    public:
    virtual void loadFromMap(
        const QMap<QString,QVariant>& map
        ,int version
    ) override final;


    /*!
     * Getter method.
     *
     * @return
     * This block's parents property.
     */
    public:
    const QStringList& parents(
    ) const;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    /*!
     * Sets this block's parents property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setParents(
        const QStringList& value
    );


    /*!
     * Set this block's templates property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setTemplates(
        const QStringList& value
    );


    /*!
     * Getter method.
     *
     * @return
     * This block's templates property.
     */
    public:
    const QStringList& templates(
    ) const;


    /*!
     * Forces this instance to update its display icon property. The given
     * ignore pointer is used to ignore any matching child function block in
     * determining if this class is concrete, virtual, or abstract.
     *
     * @param ignore
     *        The ignore pointer.
     */
    public:
    void updateDisplayIcon(
        const Block::Abstract* ignore = nullptr
    );


    /*!
     * Getter method.
     *
     * @return
     * The class icon.
     */
    private:
    static const QIcon* icon(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract class icon.
     */
    private:
    static const QIcon* iconAbstract(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual class icon.
     */
    private:
    static const QIcon* iconVirtual(
    );


    /*!
     * Sets this instance's display icon property to the value at the given
     * memory location. The given memory location must be a valid Qt icon.
     *
     * @param pointer
     *        The memory location.
     */
    private:
    void setDisplayIcon(
        const QIcon* pointer
    );
};
}
}


#endif
