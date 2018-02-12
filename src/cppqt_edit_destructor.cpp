#include <QFormLayout>
#include <QLabel>
#include "cppqt_edit_destructor.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Destructor::Destructor(AbstractBlock* block, QWidget* parent):
   Function(block,parent)
{}






QLayout* Destructor::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Basic Information"));
   Base::addDescription(ret);
   addOperations(ret);
   addTitle(ret,tr("Properties"));
   addNoExcept(ret);
   addVirtual(ret);
   addAbstract(ret);
   addOverride(ret);
   addFinal(ret);
   return ret;
}
