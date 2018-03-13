#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H
#include <memory>
#include <QtCore>
#include "global.h"
//



/*!
 */
class BlockModel : public QAbstractItemModel
{
   Q_OBJECT
public:
   explicit BlockModel(AbstractBlock* root = nullptr, QObject* parent = nullptr);
   virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override final;
   virtual QModelIndex parent(const QModelIndex& child) const override final;
   virtual int rowCount(const QModelIndex& parent) const override final;
   virtual int columnCount(const QModelIndex& parent) const override final;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
   AbstractBlock* pointer(const QModelIndex& index) const;
   bool insert(const QModelIndex& index, std::unique_ptr<AbstractBlock>&& block);
   QModelIndex moveUp(const QModelIndex& index);
   QModelIndex moveDown(const QModelIndex& index);
   bool remove(const QModelIndex& index);
   std::unique_ptr<AbstractBlock> copy(const QModelIndex& index) const;
   std::unique_ptr<AbstractBlock> cut(const QModelIndex& index);
   const AbstractBlockFactory* factory() const;
   void setRoot(AbstractBlock* newRoot);
private slots:
   void blockNameChanged(AbstractBlock* block);
private:
   /*!
    */
   constexpr static int _iconSize {24};
   /*!
    */
   AbstractBlock* _root;
   /*!
    */
   const AbstractBlockFactory* _factory {nullptr};
};



#endif
