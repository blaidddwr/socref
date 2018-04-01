#include "cppqt_edit_namespace.h"
#include <QFormLayout>
#include "cppqt_namespace.h"



using namespace std;
using namespace CppQt::Edit;






Namespace::Namespace(CppQt::Namespace* block, QWidget *parent):
   Base(block,parent)
{}






QLayout* Namespace::layout()
{
   saveSettings("cppqt.edit.namespace.geometry");
   QFormLayout* ret {new QFormLayout};
   Base::addFields(ret);
   return ret;
}
