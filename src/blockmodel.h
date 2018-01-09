#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H
#include <memory>

#include <QtCore>

#include "classes.h"



class BlockModel : public QAbstractItemModel
{
   Q_OBJECT
public:
   explicit BlockModel(AbstractBlock* root = nullptr, QObject* parent = nullptr);
   virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override final;
   virtual QModelIndex parent(const QModelIndex &child) const override final;
   virtual int rowCount(const QModelIndex& parent) const override final;
   virtual int columnCount(const QModelIndex&) const override final { return 1; }
   AbstractBlock* pointer(const QModelIndex& index) const;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
   bool insertRow(int row, const QModelIndex& parent, std::unique_ptr<AbstractBlock>&& object);
   bool moveRow(int source, int destination, const QModelIndex& parent);
   bool removeRow(int row, const QModelIndex& parent);
   std::unique_ptr<AbstractBlock> copyRow(int row, const QModelIndex& parent) const;
   std::unique_ptr<AbstractBlock> cutRow(int row, const QModelIndex& parent);
   const AbstractBlockFactory* factory() const { return _factory; }
   BlockModel& setRoot(AbstractBlock* root);
private slots:
   void blockNameChanged(AbstractBlock* object);
private:
   AbstractBlock* _root;
   const AbstractBlockFactory* _factory {nullptr};
};



#endif
