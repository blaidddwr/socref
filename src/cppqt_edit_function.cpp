#include <QGroupBox>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
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
   ret->addWidget(createFlagsEdit());
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






QGroupBox* Function::createFlagsEdit()
{
   createFlagWidgets();
   QGroupBox* ret {new QGroupBox(tr("Flags"))};
   QGridLayout* layout {new QGridLayout};
   layout->addWidget(createLabel(tr("Pre:"),Qt::AlignCenter),0,0);
   layout->addWidget(_preFlags,0,1);
   layout->addWidget(createLabel(tr("Post:"),Qt::AlignCenter),1,0);
   layout->addWidget(_postFlags);
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
   _block->setPreFlags(_preFlags->text());
   _block->setPostFlags(_postFlags->text());
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






void Function::createFlagWidgets()
{
   _preFlags = new QLineEdit;
   _postFlags = new QLineEdit;
   _preFlags->setText(_block->preFlags());
   _preFlags->setValidator(new QRegExpValidator(QRegExp(CppQt::Function::_preFlagRegExp)));
   _postFlags->setText(_block->postFlags());
   _postFlags->setValidator(new QRegExpValidator(QRegExp(CppQt::Function::_postFlagRegExp)));
}
