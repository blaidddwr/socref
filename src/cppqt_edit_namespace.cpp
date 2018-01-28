#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include "cppqt_namespace.h"
#include "cppqt_edit_namespace.h"
#include "exception.h"
#include "cppqt_gui_typelistdialog.h"



using namespace std;
using namespace Gui;
using namespace CppQt::Edit;
using namespace CppQt::Gui;






Namespace::Namespace(AbstractBlock* block, QWidget *parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Namespace*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   QVBoxLayout* layout {new QVBoxLayout};
   QGroupBox* basic {new QGroupBox(tr("Basic Information"))};
   QGroupBox* types {new QGroupBox(tr("Types"))};
   basic->setLayout(Base::layout());
   types->setLayout(createTypeButtons());
   layout->addWidget(basic);
   layout->addWidget(types);
   layout->addStretch();
   _layout = layout;
}






QLayout* Namespace::layout()
{
   return _layout;
}






void Namespace::okClicked()
{
   applyClicked();
   finished();
}






void Namespace::applyClicked()
{
   Base::applyClicked();
}






void Namespace::cancelClicked()
{
   emit finished();
}






void Namespace::editGlobalTypesClicked()
{
   TypeListDialog dialog(_block->root());
   dialog.setWindowTitle(tr("Edit Global Type List"));
   dialog.exec();
}






void Namespace::editLocalTypesClicked()
{
   TypeListDialog dialog(_block);
   dialog.setWindowTitle(tr("Edit Local Type List"));
   dialog.exec();
}






QLayout* Namespace::createTypeButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   QPushButton* global {new QPushButton(tr("Global"))};
   QPushButton* local {new QPushButton(tr("Local"))};
   ret->addStretch();
   ret->addWidget(global);
   ret->addWidget(local);
   ret->addStretch();
   connect(global,&QPushButton::clicked,this,&Namespace::editGlobalTypesClicked);
   connect(local,&QPushButton::clicked,this,&Namespace::editLocalTypesClicked);
   return ret;
}
