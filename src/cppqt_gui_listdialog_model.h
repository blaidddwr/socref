#ifndef CPPQT_GUI_LISTDIALOG_MODEL_H
#define CPPQT_GUI_LISTDIALOG_MODEL_H
#include <QAbstractListModel>
#include "cppqt_gui_listdialog.h"



namespace CppQt
{
   namespace Gui
   {
      class ListDialog::Model : public QAbstractListModel
      {
         Q_OBJECT
      public:
         explicit Model(QStringList* list = nullptr, QObject* parent = nullptr);
         virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
         virtual int rowCount(const QModelIndex& parent) const override final;
         virtual QVariant data(const QModelIndex& index, int role) const override final;
         virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override final;
         bool addAfter(const QModelIndex& index);
         bool remove(const QModelIndex& index);
         QModelIndex moveUp(const QModelIndex& index);
         QModelIndex moveDown(const QModelIndex& index);
         void setStringList(QStringList* list);
      private:
         QStringList* _list;
      };
   }
}



#endif
