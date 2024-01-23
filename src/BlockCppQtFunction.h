#ifndef BLOCK_CPPQT_FUNCTION_H
#define BLOCK_CPPQT_FUNCTION_H
#include "BlockCppFunction.h"
namespace Block {
namespace CppQt {




/*!
 * This is a C++/Qt block class. It represents a C++/Qt function.
 * 
 * It extends the C++ function with the Qt concepts of signals and slots. It
 * also implements the Qt invokable specifier which allows methods to be called
 * from the Qt meta object system.
 */
class Function:
    public Block::Cpp::Function
{
    Q_OBJECT
    public:
    using Cpp::Function::Function;


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    /*!
     * Determines if this function is invokable from Qt's meta object system.
     *
     * @return
     * True if it is invokable otherwise false.
     */
    public:
    bool isQtInvokable(
    ) const;


    /*!
     * Determines if this function is a Qt signal.
     *
     * @return
     * True if it is a Qt signal otherwise false.
     */
    public:
    bool isSignal(
    ) const;


    /*!
     * Determines if this function is a Qt slot.
     *
     * @return
     * True if it is a Qt slot otherwise false.
     */
    public:
    bool isSlot(
    ) const;


    protected:
    virtual void appendLeftFlags(
        QStringList& words
    ) const override final;


    protected:
    virtual void appendReturn(
        QStringList& words
    ) const override final;


    protected:
    virtual void appendSignature(
        QStringList& words
    ) const override final;


    protected:
    virtual void check(
    ) const override final;


    protected:
    virtual Block::Abstract* create(
        QObject* parent = nullptr
    ) const override final;


    protected:
    virtual const QMap<int,QString>& flagStringMap(
    ) const override final;


    protected:
    virtual int loadFlagsLegacy(
        const QMap<QString,QVariant>& map
    ) override final;


    protected:
    virtual const QStringList& typeStrings(
    ) const override final;


    protected:
    virtual void updateDisplayIcon(
    ) override final;


    /*!
     * Checks to see if this instance's current properties make a valid Qt
     * signal.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkSignal(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid Qt slot.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkSlot(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The abstract protected slot icon.
     */
    private:
    static const QIcon* iconAbstractSlotProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract public slot icon.
     */
    private:
    static const QIcon* iconAbstractSlotPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The signal icon.
     */
    private:
    static const QIcon* iconSignal(
    );


    /*!
     * Getter method.
     *
     * @return
     * The private slot icon.
     */
    private:
    static const QIcon* iconSlotPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The protected slot icon.
     */
    private:
    static const QIcon* iconSlotProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The public slot icon.
     */
    private:
    static const QIcon* iconSlotPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual protected slot icon.
     */
    private:
    static const QIcon* iconVirtualSlotProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual public slot icon.
     */
    private:
    static const QIcon* iconVirtualSlotPublic(
    );
};
}
}


#endif
