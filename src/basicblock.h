#ifndef BASICBLOCK_H
#define BASICBLOCK_H
#include "abstractblock.h"
//



/*!
 * This implements the abstract block class. This is designed as a basic block that 
 * reads in its type configuration from an XML element supplied to it from its 
 * corresponding factory class. The XML definition provides the basic block type 
 * information about all data fields it stores, all basic information, the build 
 * list for its block type, and definitions for its basic editor and viewer. 
 * Instances of this class must be initialized before they can be used. Two 
 * additional interfaces are defined in this class for implementations to help in 
 * creating copies of itself and defining possible variable types if its project 
 * type uses hardly typed variables. 
 * 
 * This class can be inherited and most abstract block functionality can be 
 * overridden except for any interface related to the block's data, basic 
 * interfaces for getting the block's type and factory, and making the editor for 
 * basic blocks. Any block class inheriting from this basic block class cannot add 
 * additional data, all of which must be defined within the basic block type. 
 * 
 * This class provides protected helper methods for getting the values of its data 
 * fields. These are intended to be used with specialized implementations of the 
 * basic block class that require to know the value of certain data fields. Access 
 * to the data fields are read only using these methods. 
 */
class BasicBlock : public AbstractBlock
{
   Q_OBJECT
public:
   class Edit;
   class View;
public:
   /*!
    * Defines all possible field types for basic block data. 
    */
   enum class Field
   {
      /*!
       * Defines the boolean field type. 
       */
      Bool
      /*!
       * Defines the string field type. 
       */
      ,String
      /*!
       * Defines the string list field type. 
       */
      ,StringList
   };
public:
   virtual int type() const override final;
   virtual const AbstractBlockFactory& factory() const override final;
   virtual QString name() const override;
   virtual QIcon icon() const override;
   virtual QList<int> buildList() const override;
   virtual Sut::QPtr<QWidget> makeView() const override;
   virtual Sut::QPtr<Gui::AbstractEdit> makeEdit() override final;
public:
   void initialize(int type, const AbstractBlockFactory* factory, const QDomElement& element, const QList<int>& buildList, bool isDefault);
protected:
   virtual void readData(const QDomElement& element) override final;
   virtual QDomElement writeData(QDomDocument& document) const override final;
   virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
   virtual Sut::QPtr<BasicBlock> makeBlankBasic() const;
   virtual void copyDataFrom(const AbstractBlock* other) override final;
   virtual QJsonObject typeList() const;
protected:
   bool getBool(const QString& id);
   QString getString(const QString& id);
   QStringList getStringList(const QString& id);
private:
   /*!
    * The boolean tag name of elements that define a boolean data field. 
    */
   static const char* _boolTag;
   /*!
    * The string tag name of elements that define a string data field. 
    */
   static const char* _stringTag;
   /*!
    * The string list tag name of elements that define a string list data field. 
    */
   static const char* _stringListTag;
   /*!
    * The tag name of the element used to define the name option that specifies which 
    * string field is used for this block's name. 
    */
   static const char* _nameTag;
   /*!
    * The tag name of the element used to define the path for this basic block type's 
    * icon. 
    */
   static const char* _iconTag;
   /*!
    * The tag name of the element that hold the configuration for this basic block 
    * type's edit class. 
    */
   static const char* _editTag;
   /*!
    * The tag name of the element that hold the configuration for this basic block 
    * type's view class. 
    */
   static const char* _viewTag;
   /*!
    * The attribute name for the id of data field element definitions. 
    */
   static const char* _idKey;
   /*!
    * The attribute name for default values of data field element definitions. 
    */
   static const char* _defaultKey;
private:
   void check() const;
   void addField(Field type, const QDomElement& element, bool isDefault);
   QVariant get(const QString& id);
   /*!
    * XML element that defines this basic block type. Used to make copies of this 
    * basic block. 
    */
   QDomElement _definition;
   /*!
    * XML element that defines the editor of this basic block type. 
    */
   QDomElement _editDefinition;
   /*!
    * XML element that defines the viewer of this basic block type. 
    */
   QDomElement _viewDefinition;
   /*!
    * The block type of this basic block. Also used to keep track of this basic block 
    * being initialized or not. If it is not initialized it is -1 else it is its block 
    * type. 
    */
   int _type {-1};
   /*!
    * The basic block factory that made this basic block. 
    */
   const AbstractBlockFactory* _factory;
   /*!
    * Data fields of this basic block mapped with their string ids. 
    */
   QMap<QString,QVariant> _fields;
   /*!
    * The build list for this basic block type. 
    */
   QList<int> _buildList;
   /*!
    * The data field id that this basic block uses for a display name. If this is 
    * empty this basic block's display name is also empty. 
    */
   QString _nameFieldId;
   /*!
    * The path to the icon for this basic block type. 
    */
   QString _iconPath;
};



#endif
