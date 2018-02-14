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
   Base::addDescription(ret);
   addOperations(ret);
   addProperties(ret);
   return ret;
}






void Constructor::addProperties(QFormLayout* layout)
{
   layout->addRow(new QLabel(tr("Properties:")),setupNoExcept());
}
