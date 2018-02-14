#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <exception.h>
#include "cppqt_edit_access.h"
#include "cppqt_access.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






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
   addCombo(ret);
   return ret;
}






bool Access::apply()
{
   _block->setAccessType(static_cast<CppQt::Access::Type>(CppQt::Access::_typeNames.indexOf(_box->itemText(_box->currentIndex()))));
   return true;
}






void Access::addCombo(QFormLayout* layout)
{
   setupCombo();
   layout->addRow(new QLabel(tr("Type:")),_box);
}






void Access::setupCombo()
{
   _box = new QComboBox;
   if ( !_block->hasSignalsOrSlots() )
   {
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::Public)));
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::Protected)));
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::Private)));
   }
   if ( !_block->hasSlots() && !_block->hasRegularMembers() )
   {
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::Signals)));
   }
   if ( !_block->hasSignals() && !_block->hasRegularMembers() )
   {
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::PublicSlots)));
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::ProtectedSlots)));
      _box->addItem(CppQt::Access::_typeNames.at(static_cast<int>(CppQt::Access::Type::PrivateSlots)));
   }
}
