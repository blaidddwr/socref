#ifndef BASICBLOCK_H
#define BASICBLOCK_H
#include "abstractblock.h"
//



/*!
 */
class BasicBlock : public AbstractBlock
{
public:
   class Edit;
   class View;
public:
   virtual int type() const override final;
   virtual const AbstractBlockFactory& factory() const override final;
   virtual QString name() const override;
   virtual QIcon icon() const override;
   virtual QList<int> buildList() const override final;
   virtual Sut::QPtr<QWidget> makeView() const override;
   virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override;
public:
   void initialize(const QDomElement& element, const QList<int>& _buildList, bool isDefault);
protected:
   virtual void readData(const QDomElement& element) override final;
   virtual QDomElement writeData(QDomDocument& document) const override final;
   virtual Sut::QPtr<AbstractBlock> makeBlank() const override;
   virtual void copyDataFrom(const AbstractBlock* other) override final;
private:
   /*!
    */
   QDomElement _editDefinition;
   /*!
    */
   QDomElement _viewDefinition;
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
