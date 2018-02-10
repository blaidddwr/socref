#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <exception.h>
#include "cppqt_edit_access.h"
#include "cppqt_access.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;
const QStringList Access::_options {"public","protected","private","signals","public slots","protected slots","private slots"};






Access::Access(AbstractBlock* block, QWidget* parent):
   Gui::AbstractEdit(parent),
   _block(qobject_cast<CppQt::Access*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Access::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,"Basic");
   addComboBox(ret);
   return ret;
}






void Access::applyClicked()
{
   _block->setAccessType(static_cast<CppQt::Access::Type>(_options.indexOf(_box->itemText(_box->currentIndex()))));
}






void Access::addComboBox(QFormLayout* layout)
{
   _box = new QComboBox;
   if ( !_block->hasSignalsOrSlots() )
   {
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::Public)));
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::Protected)));
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::Private)));
   }
   if ( !_block->hasSlots() && !_block->hasFunctionsOrVariables() )
   {
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::Signals)));
   }
   if ( !_block->hasSignals() && !_block->hasFunctionsOrVariables() )
   {
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::PublicSlots)));
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::ProtectedSlots)));
      _box->addItem(_options.at(static_cast<int>(CppQt::Access::Type::PrivateSlots)));
   }
   layout->addRow(new QLabel(tr("Type:")),_box);
}
