#include <QGroupBox>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QRegExpValidator>
#include "cppqt_edit_function.h"
#include "cppqt_function.h"
#include "exception.h"
#include "cppqt_gui_typecombobox.h"



using namespace CppQt::Edit;






Function::Function(AbstractBlock* block, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Function*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Function::layout()
{
   QVBoxLayout* ret {new QVBoxLayout};
   QGroupBox* basic {new QGroupBox(tr("Basic Information"))};
   basic->setLayout(Base::layout());
   ret->addWidget(basic);
   ret->addWidget(createPropertiesEdit());
   ret->addWidget(createReturnEdit());
   return ret;
}






QGroupBox* Function::createReturnEdit()
{
   createReturnWidgets();
   QGroupBox* ret {new QGroupBox(tr("Return"))};
   QGridLayout* layout {new QGridLayout};
   layout->addWidget(createLabel(tr("Type:"),Qt::AlignCenter),0,0);
   layout->addWidget(_returnCombo,0,1);
   layout->addWidget(createLabel(tr("Description:")),1,0);
   layout->addWidget(_returnEdit);
   ret->setLayout(layout);
   return ret;
}






QGroupBox* Function::createPropertiesEdit()
{
   createPropertiesWidgets();
   QGroupBox* ret {new QGroupBox(tr("Properties"))};
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_virtualBox);
   layout->addWidget(_staticBox);
   layout->addWidget(_constBox);
   layout->addWidget(_overrideBox);
   layout->addWidget(_finalBox);
   layout->addWidget(_abstractBox);
   ret->setLayout(layout);
   return ret;
}






void Function::okClicked()
{
   applyClicked();
   emit finished();
}






void Function::applyClicked()
{
   Base::applyClicked();
   _block->setReturnType(_returnCombo->value());
   _block->setReturnDescription(_returnEdit->toPlainText());
   _block->setVirtual(_virtualBox->isChecked());
   _block->setStatic(_staticBox->isChecked());
   _block->setConst(_constBox->isChecked());
   _block->setOverride(_overrideBox->isChecked());
   _block->setFinal(_finalBox->isChecked());
   _block->setAbstract(_abstractBox->isChecked());
}






void Function::cancelClicked()
{
   emit finished();
}






void Function::createReturnWidgets()
{
   _returnCombo = new Gui::TypeComboBox(_block);
   _returnEdit = new QPlainTextEdit;
   _returnCombo->setCurrentIndex(_block->returnType());
   _returnEdit->setPlainText(_block->returnDescription());
}






void Function::createPropertiesWidgets()
{
   _virtualBox = new QCheckBox(tr("Virtual"));
   _staticBox = new QCheckBox(tr("Static"));
   _constBox = new QCheckBox(tr("Constant"));
   _overrideBox = new QCheckBox(tr("Override"));
   _finalBox = new QCheckBox(tr("Final"));
   _abstractBox = new QCheckBox(tr("Abstract(Pure Virtual)"));
   _virtualBox->setChecked(_block->isVirtual());
   _staticBox->setChecked(_block->isStatic());
   _constBox->setChecked(_block->isConst());
   _overrideBox->setChecked(_block->isOverride());
   _finalBox->setChecked(_block->isFinal());
   _abstractBox->setChecked(_block->isAbstract());
}
