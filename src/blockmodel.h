#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H
#include <QtCore>
#include <socutil/soc_ut_qptr.h>
#include "abstract.h"
//



/*!
 * This is the abstract item model for the entire block tree of a project, using 
 * the Qt abstract item model class. It only implements the interfaces necessary 
 * for read only functionality through Qt view classes. However the writable 
 * functionality exists with custom functions outside the Qt framework. The custom 
 * write operations supported are inserting blocks, moving blocks up and down, 
 * removing blocks, copying blocks, and cutting blocks. 
 */
class BlockModel : public QAbstractItemModel
{
   Q_OBJECT
public:
   virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override final;
   virtual QModelIndex parent(const QModelIndex& child) const override final;
   virtual int rowCount(const QModelIndex& index) const override final;
   virtual int columnCount(const QModelIndex& parent) const override final;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
public:
   explicit BlockModel(QObject* parent = nullptr);
   Abstract::Block* pointer(const QModelIndex& index) const;
   bool insert(const QModelIndex& index, Soc::Ut::QPtr<Abstract::Block>&& block);
   QModelIndex moveUp(const QModelIndex& index);
   QModelIndex moveDown(const QModelIndex& index);
   bool remove(const QModelIndex& index);
   Soc::Ut::QPtr<Abstract::Block> copy(const QModelIndex& index) const;
   Soc::Ut::QPtr<Abstract::Block> cut(const QModelIndex& index);
   const Abstract::BlockFactory* factory() const;
   void setRoot(Abstract::Block* root);
private slots:
   void blockUpdated(Abstract::Block* block);
private:
   /*!
    * A pointer to the root block for this model's data. If this model is empty then 
    * this is null. 
    */
   Abstract::Block* _root {nullptr};
   /*!
    * A read only pointer to this model block type's block factory. If this model is 
    * empty then this is null. 
    */
   const Abstract::BlockFactory* _factory {nullptr};
};



#endif
