#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSpinBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>

#include "cppqt_edit_definition.h"
#include "cppqt_definition.h"
#include "cppqt_type_edit.h"
#include "exception.h"



using namespace std;
using namespace CppQt::Edit;






Definition::Definition(AbstractBlock* block, QWidget* parent):
   Gui::AbstractEdit(parent),
   _block(dynamic_cast<CppQt::Definition*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}

void Definition::okClicked() {}

void Definition::applyClicked()
{
   if ( _typeEdit->isValid(true) )
   {
      _block->setType(_typeEdit->type());
   }
}

unique_ptr<QLayout> Definition::createForm()
{
   unique_ptr<QLayout> ret{new QHBoxLayout};
   _typeEdit = new Type::Edit(_block->getType());
   ret->addWidget(_typeEdit);
   return ret;
}
