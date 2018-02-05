#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
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
}






QLayout* Namespace::layout()
{
   QFormLayout* ret {new QFormLayout};
   ret->addRow(createTitle(tr("Basic Information")));
   Base::addFields(ret);
   ret->addRow(createTitle(tr("Types")));
   addTypeButtons(ret);
   return ret;
}






void Namespace::okClicked()
{
   applyClicked();
   emit finished();
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






void Namespace::addTypeButtons(QFormLayout* layout)
{
   QPushButton* global {new QPushButton(tr("Global Types"))};
   QPushButton* local {new QPushButton(tr("Local Types"))};
   connect(global,&QPushButton::clicked,this,&Namespace::editGlobalTypesClicked);
   connect(local,&QPushButton::clicked,this,&Namespace::editLocalTypesClicked);
   layout->addRow(global);
   layout->addRow(local);
}
