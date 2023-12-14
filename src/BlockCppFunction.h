#ifndef BLOCK_CPP_FUNCTION_H
#define BLOCK_CPP_FUNCTION_H
#include "BlockCpp.h"
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ function.
 * 
 * Its properties are type, assignment, access, return type, return description,
 * templates, and flags. The return type and description are self-explanatory.
 * The assignment is what a function is assigned, if anything, such as default
 * or abstract. The templates list is a list of template arguments for a
 * function. The type is the type of a function. The access is the access
 * permissions of a function. The flags are the different possible flags of a
 * function. For example, constant and static are two possible flags.
 * 
 * The property type for the assignment, type, access, and flags properties are
 * integers to allow for expansion. If another language, such as C++/Qt, can
 * inherit a C++ function for its own version of a function but require
 * expansion of any of these properties it can be done by making their own
 * enumeration that begins with the C++ enumeration's user role.
 * 
 * There are numerous virtual methods that can be overridden to expand the
 * property types which allow expansion. These methods deal with formatting
 * these properties into strings, loading them from strings, and saving them to
 * strings.
 * 
 * A words list is generated in order to correctly build the definition of a
 * function. There are many protected methods called to add to this word list
 * based off the properties that can be expanded upon.
 */
class Function:
    public Block::Cpp::Base
{
    Q_OBJECT
    QString _returnDescription;
    QString _returnType {"void"};
    QStringList _templates;
    int _access {PublicAccess};
    int _assignment {NoFunctionAssignment};
    int _flags {0};
    int _type {RegularFunctionType};
    static const QHash<QString,int> _REVERSE_FLAG_LOOKUP;
    static const QHash<int,QString> _FLAG_STRINGS;
    static const QStringList _ACCESS_STRINGS;
    static const QStringList _ASSIGNMENT_STRINGS;
    static const QStringList _TYPE_STRINGS;
    public:
    using ::Block::Cpp::Base::Base;


    /*!
     * Signals this instance's access property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void accessChanged(
        int value
    );


    /*!
     * Signals this instance's assignment property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void assignmentChanged(
        int value
    );


    /*!
     * Signals this instance's flags property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void flagsChanged(
        int value
    );


    /*!
     * Signals this instance's return description property has changed to the
     * given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void returnDescriptionChanged(
        const QString& value
    );


    /*!
     * Signals this instance's return type property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void returnTypeChanged(
        const QString& value
    );


    /*!
     * Signals this instance's templates property has changed to the given
     * value.
     *
     * @param value
     *        The value.
     */
    signals:
    void templatesChanged(
        const QStringList& value
    );


    /*!
     * Signals this instance's type property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    signals:
    void typeChanged(
        int value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's access property.
     */
    public:
    int access(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's access property as a string.
     */
    public:
    virtual QString accessString(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's assignment property.
     */
    public:
    int assignment(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's assignment property as a string.
     */
    public:
    virtual QString assignmentString(
    ) const;


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    public:
    virtual QString displayText(
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * This instance's flags property as a list of strings where each string
     * represents an enabled flag.
     */
    public:
    virtual QStringList flagStrings(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's flags property.
     */
    public:
    int flags(
    ) const;


    /*!
     * Determines if this function is abstract.
     *
     * @return
     * True if it is abstract otherwise false.
     */
    public:
    bool isAbstract(
    ) const;


    /*!
     * Determines if this function is virtual.
     *
     * @return
     * True if it is virtual otherwise false.
     */
    public:
    bool isVirtual(
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
     * This instance's return description property.
     */
    public:
    const QString& returnDescription(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's type property.
     */
    public:
    const QString& returnType(
    ) const;


    public:
    virtual QMap<QString,QVariant> saveToMap(
    ) const override final;


    /*!
     * Sets this instance's access property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setAccess(
        int value
    );


    /*!
     * Sets this instance's assignment property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setAssignment(
        int value
    );


    /*!
     * Sets this instance's flags property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setFlags(
        int value
    );


    /*!
     * Sets this instance's return description property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setReturnDescription(
        const QString& value
    );


    /*!
     * Sets this instance's return type property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setReturnType(
        const QString& value
    );


    /*!
     * Sets this instance's templates property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setTemplates(
        const QStringList& value
    );


    /*!
     * Sets this instance's type property to the given value.
     *
     * @param value
     *        The value.
     */
    public:
    void setType(
        int value
    );


    /*!
     * Getter method.
     *
     * @return
     * This instance's templates property.
     */
    public:
    const QStringList& templates(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's type property.
     */
    public:
    int type(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * This instance's type property as a string.
     */
    public:
    virtual QString typeString(
    ) const;


    /*!
     * Returns the list of arguments for this function derived from its variable
     * block children. Each string in the returned list represents an individual
     * argument formatted correctly for use in code output.
     * 
     * The "only types" flag determines if only the type is returned. If it is
     * true then the name of each argument is omitted otherwise the name is
     * included.
     *
     * @param onlyTypes
     *        The "only types" flag.
     */
    protected:
    virtual QStringList arguments(
        bool onlyTypes = false
    ) const;


    /*!
     * Adds the assignment words for this function's definition to the given
     * words list.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void addAssignment(
        QStringList& words
    ) const;


    protected:
    virtual void addEvent(
    ) override final;


    /*!
     * Adds the flag(specifier) keywords for this function's definition that
     * belong on the left side of its name to the given words list.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void addLeftFlags(
        QStringList& words
    ) const;


    /*!
     * Adds the return type words to the given words list.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void addReturn(
        QStringList& words
    ) const;


    /*!
     * Adds the flag(specifier) keywords for this function's definition that
     * belong on the right side of its name to the given words list. This does
     * not include signature relevant flags which are added in the add right
     * signature flags method.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void addRightFlags(
        QStringList& words
    ) const;


    /*!
     * Adds the signature relevant flag(specifier) keywords for this function's
     * definition that belong on the right side of its name to the given words
     * list. Signature relevant means it is a flag used to make a unique
     * function signature in C++ such as constant.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void addRightSignatureFlags(
        QStringList& words
    ) const;


    /*!
     * Adds the signature words to the given words list. Signature in this
     * context means the function name and argument types enclosed within
     * parentheses.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void addSignature(
        QStringList& words
    ) const;


    /*!
     * Loads and returns the access from the given value using the given format
     * version.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    protected:
    virtual int loadAccess(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads and returns the function assignment from the given value using the
     * given format version.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    protected:
    virtual int loadAssignment(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads and returns the function assignment from the given map using the
     * legacy format version.
     *
     * @param map
     *        The map.
     */
    protected:
    virtual int loadAssignmentLegacy(
        const QMap<QString,QVariant>& map
    );


    /*!
     * Loads and returns the function flags from the given value using the given
     * format version.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    protected:
    virtual int loadFlags(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads and returns the function flags from the given map using the legacy
     * format version.
     *
     * @param map
     *        The map.
     */
    protected:
    virtual int loadFlagsLegacy(
        const QMap<QString,QVariant>& map
    );


    /*!
     * Loads and returns the function type from the given value using the given
     * format version.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    protected:
    virtual int loadType(
        const QVariant& value
        ,int version
    );


    /*!
     * Called when this block's parent class block's name changed signal is
     * emitted.
     */
    protected slots:
    void onClassNameChanged(
        const QString& value
    );


    protected:
    virtual void removeEvent(
    ) override final;


    protected:
    virtual QString scopeName(
    ) const override final;
};
}
}


#endif
