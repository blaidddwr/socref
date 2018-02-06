#include <QMenu>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include "cppqt_gui_typecombobox.h"
#include "abstractblock.h"
#include "cppqt_namespace.h"
#include "exception.h"
#include "cppqt_gui_typelistdialog.h"



using namespace std;
using namespace CppQt::Gui;






TypeComboBox::TypeComboBox(AbstractBlock* block, QWidget* parent):
   QWidget(parent)
{
   findNamespaces(block);
   _box = new QComboBox;
   buildComboList();
   QHBoxLayout* layout {new QHBoxLayout};
   layout->addWidget(_box);
   layout->addWidget(createButton());
   setLayout(layout);
}






void TypeComboBox::setCurrentIndex(const QString& type)
{
   _box->setCurrentIndex(_box->findText(type));
}






QString TypeComboBox::value() const
{
   return _box->itemText(_box->currentIndex());
}






void TypeComboBox::editGlobalTriggered()
{
   TypeListDialog dialog(_global);
   dialog.exec();
}






void TypeComboBox::editLocalTriggered()
{
   TypeListDialog dialog(_local);
   dialog.exec();
}






void TypeComboBox::typeListChanged()
{
   QString current {value()};
   buildComboList();
   setCurrentIndex(current);
}






QPushButton* TypeComboBox::createButton()
{
   QPushButton* ret {new QPushButton(tr("&Edit"))};
   ret->setMenu(createMenu());
   return ret;
}






QMenu* TypeComboBox::createMenu()
{
   QMenu* ret {new QMenu(this)};
   ret->addAction(createGlobalAction());
   ret->addAction(createLocalAction());
   return ret;
}






QAction* TypeComboBox::createGlobalAction()
{
   QAction* ret {new QAction(tr("&Global"),this)};
   ret->setStatusTip(tr("Edit global list of C++ types."));
   connect(ret,&QAction::triggered,this,&TypeComboBox::editGlobalTriggered);
   return ret;
}






QAction* TypeComboBox::createLocalAction()
{
   QAction* ret {new QAction(tr("&Local"),this)};
   ret->setStatusTip(tr("Edit local list of C++ types."));
   connect(ret,&QAction::triggered,this,&TypeComboBox::editLocalTriggered);
   return ret;
}






void TypeComboBox::findNamespaces(AbstractBlock* block)
{
   while ( !(_local = qobject_cast<Namespace*>(block)) )
   {
      block = block->parent();
      if ( !block )
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Root block reached without finding a single namespace."));
         throw e;
      }
   }
   while ( block->parent() ) block = block->parent();
   _global = qobject_cast<Namespace*>(block);
   if ( !_global )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not a namespace."));
      throw e;
   }
   if ( _local == _global ) _local = nullptr;
   else connect(_local,&Namespace::typesChanged,this,&TypeComboBox::typeListChanged);
   connect(_global,&Namespace::typesChanged,this,&TypeComboBox::typeListChanged);
}






void TypeComboBox::buildComboList()
{
   QStringList types {_global->types()};
   if ( _local ) types.append(_local->types());
   sort(types.begin(),types.end());
   _box->clear();
   for (auto type : qAsConst(types)) _box->addItem(type);
}
