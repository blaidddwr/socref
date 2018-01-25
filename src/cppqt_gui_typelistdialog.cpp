#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "cppqt_gui_typelistdialog.h"
#include "cppqt_namespace.h"
#include "cppqt_gui_typedialog.h"



using namespace CppQt;
using namespace CppQt::Gui;






TypeListDialog::TypeListDialog(Namespace* block, QWidget* parent):
   QDialog(parent),
   _block(block)
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(createTop());
   layout->addLayout(createBottomButtons());
   setLayout(layout);
}






void TypeListDialog::itemDoubleClicked(QListWidgetItem* item)
{
   TypeDialog dialog(item->text());
   dialog.setWindowTitle(tr("Edit Type"));
   if ( dialog.exec() )
   {
      item->setText(dialog.name());
   }
}






void TypeListDialog::addClicked()
{
   TypeDialog dialog;
   dialog.setWindowTitle(tr("New Type"));
   if ( dialog.exec() )
   {
      _list->addItem(dialog.name());
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
   for (int i = 0; i < _list->count() ;++i)
   {
      types << _list->item(i)->text();
   }
   _block->setTypes(types);
}






QLayout* TypeListDialog::createTop()
{
   _list = new QListWidget;
   _list->setSortingEnabled(true);
   const QStringList list {_block->types()};
   for (auto type : list)
   {
      _list->addItem(type);
   }
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_list);
   ret->addLayout(createTopButtons());
   connect(_list,&QListWidget::itemDoubleClicked,this,&TypeListDialog::itemDoubleClicked);
   return ret;
}






QLayout* TypeListDialog::createTopButtons()
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






QLayout* TypeListDialog::createBottomButtons()
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
