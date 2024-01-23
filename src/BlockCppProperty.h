#ifndef BLOCK_CPP_PROPERTY_H
#define BLOCK_CPP_PROPERTY_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ class property. While this is
 * not a concrete type defined in the C++ language, it is an abstract concept
 * used pervasively enough in classes to conceptualize it as a block. A
 * property's setter and getter are added as children to a property.
 * 
 * This also serves as a base class for the class C++ block because it shares
 * all common algorithms pertaining to holding functions and determining if it
 * is abstract, virtual, or regular.
 */
class Property:
    public Block::Cpp::Base
{
    Q_OBJECT
    const QIcon* _icon {nullptr};


    public:
    Property(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual ~Property(
    ) override;


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    /*!
     * Determines if this block is abstract.
     *
     * @return
     * True if it is abstract otherwise false.
     */
    public:
    bool isAbstract(
    ) const;


    /*!
     * Determines if this block is virtual.
     *
     * @return
     * True if it is virtual otherwise false.
     */
    public:
    bool isVirtual(
    ) const;


    /*!
     * Forces this instance to update its display icon property.
     */
    public:
    virtual void updateDisplayIcon(
    );


    /*!
     * Constructs this new instance with the given name, icon, meta, and parent.
     * The given meta must be valid and cannot be destroyed during the life of
     * this instance. The given icon must be valid.
     *
     * @param name
     *        The name.
     *
     * @param icon
     *        The icon.
     *
     * @param meta
     *        The meta.
     *
     * @param parent
     *        The parent.
     */
    protected:
    Property(
        const QString& name
        ,const QIcon* icon
        ,Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    protected:
    virtual void addEvent(
        int index
    ) override final;


    protected:
    virtual Block::Abstract* create(
        QObject* parent = nullptr
    ) const override;


    protected:
    virtual void removeEvent(
        int index
    ) override final;


    /*!
     * Sets this instance's display icon property to the value at the given
     * memory location. The given memory location must be a valid Qt icon.
     *
     * @param pointer
     *        The memory location.
     */
    protected:
    void setDisplayIcon(
        const QIcon* pointer
    );


    /*!
     * Getter method.
     *
     * @return
     * The property icon.
     */
    private:
    static const QIcon* icon(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract property icon.
     */
    private:
    static const QIcon* iconAbstract(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual property icon.
     */
    private:
    static const QIcon* iconVirtual(
    );
};
}
}


#endif
