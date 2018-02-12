#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_constructor.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Constructor::Constructor(AbstractBlock* block, QWidget* parent):
   Function(block,parent)
{}






QLayout* Constructor::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Basic Information"));
   Base::addDescription(ret);
   addOperations(ret);
   addTitle(ret,tr("Properties"));
   addNoExcept(ret);
   return ret;
}
