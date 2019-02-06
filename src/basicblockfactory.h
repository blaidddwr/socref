#ifndef BASICBLOCKFACTORY_H
#define BASICBLOCKFACTORY_H
#include <QDomElement>
#include "abstractblockfactory.h"
#include "global.h"
//



/*!
 * This mostly implements the abstract block factory. This facilitates reading in a 
 * XML configuration file that defines all basic blocks types this factory can 
 * produce. The definition of each basic block type is provided as a root level XML 
 * element. 
 * 
 * All interfaces from the abstract block factory are implemented except for the 
 * element name interface. This must be implemented by the actual project type and 
 * is used by this interface to make sure any enumeration it contains matches the 
 * XML file. The order of block types within any enumeration must match the order 
 * in the XML configuration file this basic block factory loads. 
 * 
 * A new interface is also provided for creating implementation classes of the 
 * basic block class. Basic blocks do no require to implemented and can simply be 
 * generic basic blocks. To do this simply return a null pointer with this class's 
 * interface. 
 */
class BasicBlockFactory : public AbstractBlockFactory
{
public:
   virtual int size() const override final;
   virtual QString name(int type) const override final;
   virtual int typeByElementName(const QString& elementName) const override final;
   virtual Soc::Ut::QPtr<AbstractBlock> makeBlock(int type, bool isDefault) const override final;
   virtual Soc::Ut::QPtr<AbstractBlock> makeRootBlock() const override final;
public:
   BasicBlockFactory(const QString& xmlPath);
protected:
   virtual Soc::Ut::QPtr<BasicBlock> makeBasicBlock(int type) const;
private:
   /*!
    * The name of the display attribute for XML basic block definitions. 
    */
   static const char* _displayTag;
   /*!
    * The name of the root attribute for XML basic block definitions. 
    */
   static const char* _rootTag;
   /*!
    * The tag name of the build list element for XML basic block definitions. 
    */
   static const char* _buildTag;
private:
   void read(const QString& path);
   void readDefinition(const QDomElement& element);
   void buildLists();
   QList<int> buildList(const QDomElement& element);
   bool check() const;
   /*!
    * List of display names for this factory's basic block type definitions. 
    */
   QStringList _typeDisplayNames;
   /*!
    * List of element names for this factory's list of basic block type definitions. 
    */
   QStringList _typeElementNames;
   /*!
    * List of XML elements for this factory's list of basic block type definitions. 
    */
   QList<QDomElement> _typeDefinitions;
   /*!
    * List of build lists for this factory's list of basic block type definitions. 
    */
   QList<QList<int>> _buildLists;
   /*!
    * Index to the basic block type definition that is considered the root block type 
    * for this factory's implementation's project type. 
    */
   int _rootType {-1};
   /*!
    * True if this factory has been checked for consistency with its implementation or 
    * false otherwise. 
    */
   mutable bool _isChecked {false};
};



#endif
