#ifndef GUI_LISTEDIT_MODEL_H
#define GUI_LISTEDIT_MODEL_H
#include "gui_listedit.h"
#include <QAbstractListModel>
//



namespace Gui
{
   /*!
    * This is the model for its parent list edit widget which handles modeling the 
    * underlying string list for the qt table view of its parent list edit widget. 
    */
   class ListEdit::Model : public QAbstractListModel
   {
      Q_OBJECT
   public:
      explicit Model(QObject* parent = nullptr);
      explicit Model(QStringList* list, QObject* parent = nullptr);
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
      virtual int rowCount(const QModelIndex& parent) const override final;
      virtual QVariant data(const QModelIndex& index, int role) const override final;
      virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override final;
      bool addAfter(const QModelIndex& index);
      bool remove(const QModelIndex& index);
      QModelIndex moveUp(const QModelIndex& index);
      QModelIndex moveDown(const QModelIndex& index);
      void setList(QStringList* list);
   private:
      /*!
       * Pointer to the string list this model represents. 
       */
      QStringList* _list {nullptr};
   };
}



#endif
