#include "cppqt_edit_namespace.h"
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <exception.h>
#include "cppqt_gui_typelistdialog.h"
#include "cppqt_namespace.h"
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
   Base::addFields(ret);
   addTypeButtons(ret);
   return ret;
}






void Namespace::addTypeButtons(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Global Types:")),setupGlobalButton());
   layout->addRow(new QLabel(tr("Local Types:")),setupLocalButton());
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






QWidget* Namespace::setupGlobalButton()
{
   QPushButton* ret {new QPushButton(tr("Edit"))};
   connect(ret,&QPushButton::clicked,this,&Namespace::editGlobalTypesClicked);
   return ret;
}






QWidget* Namespace::setupLocalButton()
{
   QPushButton* ret {new QPushButton(tr("Edit"))};
   connect(ret,&QPushButton::clicked,this,&Namespace::editLocalTypesClicked);
   return ret;
}
