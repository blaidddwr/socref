#ifndef BASICBLOCK_H
#define BASICBLOCK_H
#include "abstractblock.h"
//



/*!
 */
class BasicBlock : public AbstractBlock
{
   Q_OBJECT
public:
   class Edit;
   class View;
public:
   /*!
    */
   enum class Field
   {
      /*!
       */
      Bool
      /*!
       */
      ,String
      /*!
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
   virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override;
public:
   void initialize(int type, const AbstractBlockFactory* factory, const QDomElement& element, const QList<int>& buildList, bool isDefault);
protected:
   virtual void readData(const QDomElement& element) override final;
   virtual QDomElement writeData(QDomDocument& document) const override final;
   virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
   virtual Sut::QPtr<BasicBlock> makeBlankBasic() const;
   virtual void copyDataFrom(const AbstractBlock* other) override final;
protected:
   bool getBool(const QString& id);
   QString getString(const QString& id);
   QStringList getStringList(const QString& id);
private:
   QVariant get(const QString& id);
   void addField(Field type, const QDomElement& element, bool isDefault);
   /*!
    */
   QDomElement _definition;
   /*!
    */
   QDomElement _editDefinition;
   /*!
    */
   QDomElement _viewDefinition;
   /*!
    */
   int _type {-1};
   /*!
    */
   const AbstractBlockFactory* _factory;
   /*!
    */
   QMap<QString,QVariant> _fields;
   /*!
    */
   QList<int> _buildList;
   /*!
    */
   QString _nameFieldId;
   /*!
    */
   QString _iconPath;
};



#endif
