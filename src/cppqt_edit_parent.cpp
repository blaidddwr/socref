#include "cppqt_edit_parent.h"
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <exception.h>
#include "cppqt_parent.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Parent::Parent(AbstractBlock* block, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Parent*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   saveSettings("cppqt.edit.parent.geometry");
}






QLayout* Parent::layout()
{
   QFormLayout* ret {new QFormLayout};
   addAccess(ret);
   Base::addName(ret);
   addTemplate(ret);
   Base::addDescription(ret);
   return ret;
}






void Parent::apply()
{
   if ( !isValidTemplate() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The given template string is not a valid C++ template."));
      throw e;
   }
   Base::apply();
   _block->setAccess(static_cast<CppQt::Parent::Access>(_accessBox->currentIndex()));
   _block->setTemplateArgument(_templateEdit->text());
}






void Parent::templateChanged(const QString& text)
{
   Q_UNUSED(text)
   setDisabled(!isValidTemplate());
}






bool Parent::isValidTemplate()
{
   return _templateEdit->text().isEmpty() || isValidTemplateArgument(_templateEdit->text());
}






void Parent::addAccess(QFormLayout* layout)
{
   setupAccess();
   layout->addRow(new QLabel(tr("Access:")),_accessBox);
}






void Parent::addTemplate(QFormLayout* layout)
{
   setupTemplate();
   layout->addRow(new QLabel(tr("Template:")),_templateEdit);
}






void Parent::setupAccess()
{
   _accessBox = new QComboBox;
   for (auto item : CppQt::Parent::_accessNames) _accessBox->addItem(item);
   _accessBox->setCurrentIndex(static_cast<int>(_block->access()));
}






void Parent::setupTemplate()
{
   _templateEdit = new QLineEdit;
   connect(_templateEdit,&QLineEdit::textChanged,this,&Parent::templateChanged);
   _templateEdit->setText(_block->templateArgument());
}
