#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include "cppqt_namespace.h"
#include "cppqt_edit_namespace.h"
#include "exception.h"
#include "cppqt_gui_typelistdialog.h"
#include "cppqt_common.h"



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
}






QLayout* Namespace::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Basic Information"));
   Base::addFields(ret);
   addTitle(ret,tr("Types"));
   addTypeButtons(ret);
   return ret;
}






void Namespace::addTypeButtons(QFormLayout* layout)
{
   QPushButton* global {new QPushButton(tr("Global"))};
   QPushButton* local {new QPushButton(tr("Local"))};
   connect(global,&QPushButton::clicked,this,&Namespace::editGlobalTypesClicked);
   connect(local,&QPushButton::clicked,this,&Namespace::editLocalTypesClicked);
   QHBoxLayout* buttons {new QHBoxLayout};
   buttons->addWidget(global);
   buttons->addWidget(local);
   layout->addRow(buttons);
}






bool Namespace::applyClicked()
{
   return Base::applyClicked();
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
