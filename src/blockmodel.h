#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H
#include <QtCore>



class AbstractBlockFactory;
class AbstractBlock;



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
   //Qt::ItemFlags flags(const QModelIndex& index) const override final;
   bool insertRow(int row, const QModelIndex& parent, AbstractBlock* takenObject);
   bool moveRow(int source, int destination, const QModelIndex& parent);
   bool removeRow(int row, const QModelIndex& parent);
   AbstractBlock* copyRow(int row, const QModelIndex& parent) const;
   AbstractBlock* cutRow(int row, const QModelIndex& parent);
   //QStringList mimeTypes() const override final;
   //Qt::DropActions supportedDropActions() const override final;
   //QMimeData* mimeData(const QModelIndexList& indexes) const override final;
   //bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column
   //                  , const QModelIndex& parent) override final;
   const AbstractBlockFactory* factory() const { return _factory; }
   void setRoot(AbstractBlock* root);
private slots:
   void blockNameChanged(AbstractBlock* object);
private:
   AbstractBlock* _root;
   const AbstractBlockFactory* _factory {nullptr};
};



#endif
