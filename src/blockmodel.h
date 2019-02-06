#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H
#include <QtCore>
#include <socutil/soc_ut_qptr.h>
#include "global.h"
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
   /*!
    * Defines custom data roles this model uses for data changed signals. 
    */
   enum Role
   {
      /*!
       * Defines the name role which encompasses the title and icon of a block changing. 
       */
      Name = 100
      /*!
       * Defines the body role which encompasses the body of a block changing. 
       */
      ,Body = 101
   };
   virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override final;
   virtual QModelIndex parent(const QModelIndex& child) const override final;
   virtual int rowCount(const QModelIndex& index) const override final;
   virtual int columnCount(const QModelIndex& parent) const override final;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
public:
   explicit BlockModel(QObject* parent = nullptr);
   AbstractBlock* pointer(const QModelIndex& index) const;
   bool insert(const QModelIndex& index, Soc::Ut::QPtr<AbstractBlock>&& block);
   QModelIndex moveUp(const QModelIndex& index);
   QModelIndex moveDown(const QModelIndex& index);
   bool remove(const QModelIndex& index);
   Soc::Ut::QPtr<AbstractBlock> copy(const QModelIndex& index) const;
   Soc::Ut::QPtr<AbstractBlock> cut(const QModelIndex& index);
   const AbstractBlockFactory* factory() const;
   void setRoot(AbstractBlock* root);
private slots:
   void blockNameModified(AbstractBlock* block);
   void blockBodyModified(AbstractBlock* block);
private:
   void notifyChange(AbstractBlock* block, const QVector<int>& roles);
   /*!
    * The preferred icon size for indexes of this model when displayed in a view. This 
    * represents both the width and height. 
    */
   constexpr static int _iconSize {24};
   /*!
    * A pointer to the root block for this model's data. If this model is empty then 
    * this is null. 
    */
   AbstractBlock* _root {nullptr};
   /*!
    * A read only pointer to this model block type's block factory. If this model is 
    * empty then this is null. 
    */
   const AbstractBlockFactory* _factory {nullptr};
};



#endif
