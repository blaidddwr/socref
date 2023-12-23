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
 * Due to the complexity of a function's properties and all the possible
 * exceptions that would make it an invalid C++ function, almost all properties
 * must be set for a function at the same time using the set method. This method
 * in turn checks the validity of the given values.
 * 
 * There are two primary groups of virtual methods used to expand the property
 * values of the above mentioned properties. The first private mapping lists for
 * all valid values. The second provides words for constructing a function's
 * display text.
 * 
 * A words list is generated in order to correctly build the definition of a
 * function. There are many protected methods called to append to this word list
 * based off the properties. These methods are virtual and can be expanded upon
 * with new property value enumerations.
 */
class Function:
    public Block::Cpp::Base
{
    Q_OBJECT
    QString _displayText;
    QString _returnDescription;
    QString _returnType {"void"};
    QStringList _templates;
    const QIcon* _icon;
    int _access {PublicAccess};
    int _assignment {NoFunctionAssignment};
    int _flags {0};
    int _type {RegularFunctionType};
    using Base::setName;


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


    public:
    Function(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual ~Function(
    ) override;


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
    QString accessString(
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
    QString assignmentString(
    ) const;


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override;


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
    QStringList flagStrings(
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
     * Determines if this function has the constant flag.
     *
     * @return
     * True if it has the constant flag otherwise false.
     */
    public:
    bool isConstant(
    ) const;


    /*!
     * Determines if this function is a constructor.
     *
     * @return
     * True if it is a constructor otherwise false.
     */
    public:
    bool isConstructor(
    ) const;


    /*!
     * Determines if this function is default.
     *
     * @return
     * True if it is default otherwise false.
     */
    public:
    bool isDefault(
    ) const;


    /*!
     * Determines if this function is deleted.
     *
     * @return
     * True if it is deleted otherwise false.
     */
    public:
    bool isDeleted(
    ) const;


    /*!
     * Determines if this function is a destructor.
     *
     * @return
     * True if it is a destructor otherwise false.
     */
    public:
    bool isDestructor(
    ) const;


    /*!
     * Determines if this function has the explicit flag.
     *
     * @return
     * True if it has the explicit flag otherwise false.
     */
    public:
    bool isExplicit(
    ) const;


    /*!
     * Determines if this function has the final flag.
     *
     * @return
     * True if it has the final flag otherwise false.
     */
    public:
    bool isFinal(
    ) const;


    /*!
     * Determines if this function is a method.
     *
     * @return
     * True if it is a method otherwise false.
     */
    public:
    bool isMethod(
    ) const;


    /*!
     * Determines if this function has the no exception flag.
     *
     * @return
     * True if it has the no exception flag otherwise false.
     */
    public:
    bool isNoExcept(
    ) const;


    /*!
     * Determines if this function is a operator.
     *
     * @return
     * True if it is a operator otherwise false.
     */
    public:
    bool isOperator(
    ) const;


    /*!
     * Determines if this function has the override flag.
     *
     * @return
     * True if it has the override flag otherwise false.
     */
    public:
    bool isOverride(
    ) const;


    /*!
     * Determines if this function's access is private.
     *
     * @return
     * True if its access is private otherwise false.
     */
    public:
    bool isPrivate(
    ) const;


    /*!
     * Determines if this function's access is protected.
     *
     * @return
     * True if its access is protected otherwise false.
     */
    public:
    bool isProtected(
    ) const;


    /*!
     * Determines if this function's access is public.
     *
     * @return
     * True if its access is public otherwise false.
     */
    public:
    bool isPublic(
    ) const;


    /*!
     * Determines if this function has the static flag.
     *
     * @return
     * True if it has the static flag otherwise false.
     */
    public:
    bool isStatic(
    ) const;


    /*!
     * Determines if this function has the virtual flag.
     *
     * @return
     * True if it has the virtual flag otherwise false.
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
     * Sets this instance's name, return type, type, access, assignment, and
     * flags properties to the given values. If the given combination of
     * properties do not make a valid C++ function then no property values are
     * changed.
     * 
     * A logical block exception is thrown if the given set of property values
     * do not make a valid C++ function.
     *
     * @param name
     *        The name.
     *
     * @param returnType
     *        The return type.
     *
     * @param type
     *        The type.
     *
     * @param access
     *        The access.
     *
     * @param assignment
     *        The assignment.
     *
     * @param flags
     *        The flags.
     */
    public:
    void set(
        const QString& name
        ,const QString& returnType
        ,int type
        ,int access
        ,int assignment
        ,int flags
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
    QString typeString(
    ) const;


    /*!
     * Forces this instance to update its display text property.
     */
    public:
    void updateDisplayText(
    );


    /*!
     * Getter method.
     *
     * @return
     * A complete list of all valid access enumerations as strings. The order of
     * the returned string list must match the order of enumerations.
     */
    protected:
    virtual const QStringList& accessStrings(
    ) const;


    protected:
    virtual void addEvent(
        int index
    ) override final;


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
     * Appends the assignment words for this function's definition to the given
     * words list.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void appendAssignment(
        QStringList& words
    ) const;


    /*!
     * Appends the flag(specifier) words for this function's definition that
     * belong on the left side of its name to the given words list.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void appendLeftFlags(
        QStringList& words
    ) const;


    /*!
     * Appends the return type words to the given words list.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void appendReturn(
        QStringList& words
    ) const;


    /*!
     * Appends the flag(specifier) words for this function's definition that
     * belong on the right side of its name to the given words list. This does
     * not include signature relevant flags which are added in the add right
     * signature flags method.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void appendRightFlags(
        QStringList& words
    ) const;


    /*!
     * Appends the signature relevant flag(specifier) words for this function's
     * definition that belong on the right side of its name to the given words
     * list. Signature relevant means it is a flag used to make a unique
     * function signature in C++ such as constant.
     *
     * @param words
     *        The words.
     */
    protected:
    virtual void appendRightSignatureFlags(
        QStringList& words
    ) const;


    /*!
     * Appends the signature words to the given words list. Signature in this
     * context means the function name and argument types enclosed within
     * parentheses.
     *
     * @param words
     *        The words list.
     */
    protected:
    virtual void appendSignature(
        QStringList& words
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * A complete list of all valid function assignment enumerations as strings.
     * The order of the returned string list must match the order of
     * enumerations.
     */
    protected:
    virtual const QStringList& assignmentStrings(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid
     * function.
     * 
     * If this is overridden to provide additional checks then this base method
     * must be called in the overriding method.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    protected:
    virtual void check(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * A complete mapping of all valid function flags to their representation as
     * a string.
     */
    protected:
    virtual const QMap<int,QString>& flagStringMap(
    ) const;


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
     * Called when this block's parent class block's name changed signal is
     * emitted.
     */
    protected slots:
    void onClassNameChanged(
        const QString& value
    );


    /*!
     * Called when this block's name property has changed to the given value.
     *
     * @param value
     *        The value.
     */
    protected:
    virtual void onNameChanged(
        const QString& value
    ) override final;


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


    protected:
    virtual QString scopeName(
    ) const override final;


    /*!
     * Getter method.
     *
     * @return
     * A complete list of all valid function type enumerations as strings. The
     * order of the returned string list must match the order of enumerations.
     */
    protected:
    virtual const QStringList& typeStrings(
    ) const;


    /*!
     * Updates this instance's display icon property.
     */
    protected:
    virtual void updateDisplayIcon(
    );


    /*!
     * Determines if this instance's flag property is valid, containing nothing
     * but valid flags.
     *
     * @return
     * True if it is valid otherwise false.
     */
    private:
    bool areFlagsValid(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid
     * constructor.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkConstructor(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid
     * destructor.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkDestructor(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid method.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkMethod(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid
     * operator.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkOperator(
    ) const;


    /*!
     * Checks to see if this instance's current properties make a valid regular
     * function.
     * 
     * A logical block exception is thrown if this instance's properties are not
     * valid.
     */
    private:
    void checkRegular(
    ) const;


    /*!
     * Getter method.
     *
     * @return
     * The abstract protected destructor icon.
     */
    private:
    static const QIcon* iconAbstractDestructorProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract public destructor icon.
     */
    private:
    static const QIcon* iconAbstractDestructorPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract protected function icon.
     */
    private:
    static const QIcon* iconAbstractProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The abstract public function icon.
     */
    private:
    static const QIcon* iconAbstractPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The private constructor icon.
     */
    private:
    static const QIcon* iconConstructorPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The protected constructor icon.
     */
    private:
    static const QIcon* iconConstructorProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The public constructor icon.
     */
    private:
    static const QIcon* iconConstructorPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The private destructor icon.
     */
    private:
    static const QIcon* iconDestructorPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The protected destructor icon.
     */
    private:
    static const QIcon* iconDestructorProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The public destructor icon.
     */
    private:
    static const QIcon* iconDestructorPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The invalid function icon.
     */
    private:
    static const QIcon* iconInvalid(
    );


    /*!
     * Getter method.
     *
     * @return
     * The private operator icon.
     */
    private:
    static const QIcon* iconOperatorPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The protected operator icon.
     */
    private:
    static const QIcon* iconOperatorProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The public operator icon.
     */
    private:
    static const QIcon* iconOperatorPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The private function icon.
     */
    private:
    static const QIcon* iconPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The protected function icon.
     */
    private:
    static const QIcon* iconProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The public function icon.
     */
    private:
    static const QIcon* iconPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The static private function icon.
     */
    private:
    static const QIcon* iconStaticPrivate(
    );


    /*!
     * Getter method.
     *
     * @return
     * The static protected function icon.
     */
    private:
    static const QIcon* iconStaticProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The static public function icon.
     */
    private:
    static const QIcon* iconStaticPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual protected destructor icon.
     */
    private:
    static const QIcon* iconVirtualDestructorProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual public destructor icon.
     */
    private:
    static const QIcon* iconVirtualDestructorPublic(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual protected function icon.
     */
    private:
    static const QIcon* iconVirtualProtected(
    );


    /*!
     * Getter method.
     *
     * @return
     * The virtual public function icon.
     */
    private:
    static const QIcon* iconVirtualPublic(
    );


    /*!
     * Determines if this instance's access property value is valid.
     *
     * @return
     * True if it is valid otherwise false.
     */
    private:
    bool isAccessValid(
    ) const;


    /*!
     * Determines if this instance's assignment property value is valid.
     *
     * @return
     * True if it is valid otherwise false.
     */
    private:
    bool isAssignmentValid(
    ) const;


    /*!
     * Determines if this instance's type property value is valid.
     *
     * @return
     * True if it is valid otherwise false.
     */
    private:
    bool isTypeValid(
    ) const;


    /*!
     * Loads this instance's access property from the given value using the
     * given format version.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    private:
    void loadAccess(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads this instance's assignment property from the given value using the
     * given format version. This cannot load data from the legacy format.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    private:
    void loadAssignment(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads this instance's flags property from the given value using the given
     * format version. This cannot load data from the legacy format.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    private:
    void loadFlags(
        const QVariant& value
        ,int version
    );


    /*!
     * Loads this instance's type property from the given value using the given
     * format version.
     * 
     * A read block exception is thrown if any error is encountered.
     *
     * @param value
     *        The value.
     *
     * @param version
     *        The format version.
     */
    private:
    void loadType(
        const QVariant& value
        ,int version
    );


    /*!
     * Returns the reverse flag lookup hash table. The first time this is called
     * the hash table is constructed.
     */
    private:
    const QHash<QString,int>& reverseFlagLookup(
    ) const;


    /*!
     * Sets this instance's access property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setAccess(
        int value
    );


    /*!
     * Sets this instance's assignment property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setAssignment(
        int value
    );


    /*!
     * Sets this instance's flags property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setFlags(
        int value
    );


    /*!
     * Sets this instance's return type property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setReturnType(
        const QString& value
    );


    /*!
     * Sets this instance's type property to the given value.
     *
     * @param value
     *        The value.
     */
    private:
    void setType(
        int value
    );
};
}
}


#endif
