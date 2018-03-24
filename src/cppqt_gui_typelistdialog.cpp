#include "cppqt_gui_typelistdialog.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include "cppqt_gui_typedialog.h"
#include "cppqt_namespace.h"
#include "application.h"



using namespace CppQt;
using namespace CppQt::Gui;






TypeListDialog::TypeListDialog(Namespace* block, QWidget* parent):
   ::Gui::PersistentDialog("cppqt.gui.typelistdialog.geometry",parent),
   _block(block)
{
   setupGui();
}






void TypeListDialog::itemDoubleClicked(QListWidgetItem* item)
{
   TypeDialog dialog(item->text());
   dialog.setWindowTitle(tr("Edit Type"));
   while (true)
   {
      if ( dialog.exec() )
      {
         QString type {dialog.name()};
         if ( item->text() == type || !isDuplicate(type) )
         {
            item->setText(type);
            break;
         }
      }
      else break;
   }
}






void TypeListDialog::addClicked()
{
   TypeDialog dialog;
   dialog.setWindowTitle(tr("New Type"));
   while (true)
   {
      if ( dialog.exec() )
      {
         QString type {dialog.name()};
         if ( !isDuplicate(type) )
         {
            _list->addItem(type);
            break;
         }
      }
      else break;
   }
}






void TypeListDialog::removeClicked()
{
   delete _list->currentItem();
}






void TypeListDialog::okClicked()
{
   applyClicked();
   accept();
}






void TypeListDialog::applyClicked()
{
   QStringList types;
   for (int i = 0; i < _list->count() ;++i) types << _list->item(i)->text();
   _block->setTypes(types);
}






bool TypeListDialog::isDuplicate(const QString& name)
{
   auto alertOfDuplicate = [](const QString& name)
   {
      QMessageBox box;
      box.setWindowTitle(tr("Duplicate Type Name"));
      box.setText(tr("The type name given already exists in the %1 list.").arg(name));
      box.setIcon(QMessageBox::Warning);
      box.addButton(tr("Ok"),QMessageBox::AcceptRole);
      box.exec();
   };
   bool ret {false};
   if ( _block != _block->root() && _block->root()->types().contains(name) )
   {
      (*alertOfDuplicate)("global");
      ret = true;
   }
   else if ( !(_list->findItems(name,Qt::MatchExactly).isEmpty()) )
   {
      (*alertOfDuplicate)("local");
      ret = true;
   }
   return ret;
}






void TypeListDialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupTop());
   layout->addLayout(setupBottomButtons());
   setLayout(layout);
}






QLayout* TypeListDialog::setupTop()
{
   _list = new QListWidget;
   _list->setSortingEnabled(true);
   const QStringList list {_block->types()};
   for (auto type : list) _list->addItem(type);
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_list);
   ret->addLayout(setupTopButtons());
   connect(_list,&QListWidget::itemDoubleClicked,this,&TypeListDialog::itemDoubleClicked);
   return ret;
}






QLayout* TypeListDialog::setupTopButtons()
{
   QPushButton* add {new QPushButton(tr("Add"))};
   QPushButton* remove {new QPushButton(tr("Remove"))};
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(add);
   ret->addWidget(remove);
   ret->addStretch();
   connect(add,&QPushButton::clicked,this,&TypeListDialog::addClicked);
   connect(remove,&QPushButton::clicked,this,&TypeListDialog::removeClicked);
   return ret;
}






QLayout* TypeListDialog::setupBottomButtons()
{
   QPushButton* ok {new QPushButton(tr("Ok"))};
   QPushButton* apply {new QPushButton(tr("Apply"))};
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);
   connect(ok,&QPushButton::clicked,this,&TypeListDialog::okClicked);
   connect(apply,&QPushButton::clicked,this,&TypeListDialog::applyClicked);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   return ret;
}
