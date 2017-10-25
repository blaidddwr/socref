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
   QModelIndex index(int row, int column, const QModelIndex &parent) const override final;
   QModelIndex parent(const QModelIndex &child) const override final;
   int rowCount(const QModelIndex& parent) const override final;
   int columnCount(const QModelIndex& parent) const override final;
   QVariant data(const QModelIndex& index, int role) const override final;
   //Qt::ItemFlags flags(const QModelIndex& index) const override final;
   bool insertRow(int row, const QModelIndex& parent, AbstractBlock* object);
   bool moveRow(int source, int destination, const QModelIndex& parent);
   bool removeRows(int row, int count, const QModelIndex& parent) override final;
   //QStringList mimeTypes() const override final;
   //Qt::DropActions supportedDropActions() const override final;
   //QMimeData* mimeData(const QModelIndexList& indexes) const override final;
   //bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column
   //                  , const QModelIndex& parent) override final;
   const AbstractBlockFactory* getFactory() const;
   void setRoot(AbstractBlock* root);
private slots:
   void blockNameChanged(AbstractBlock* object);
private:
   AbstractBlock* _root;
   const AbstractBlockFactory* _factory {nullptr};
};



//@@
inline int BlockModel::columnCount(const QModelIndex&) const { return 1; }

//@@
inline const AbstractBlockFactory* BlockModel::getFactory() const { return _factory; }



#endif
