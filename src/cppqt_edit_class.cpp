#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <exception.h>
#include "cppqt_edit_class.h"
#include "cppqt_class.h"
#include "cppqt_common.h"



using namespace CppQt::Edit;






Class::Class(AbstractBlock* block, QWidget* parent):
   Namespace(block,parent),
   _block(qobject_cast<CppQt::Class*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
}






QLayout* Class::layout()
{
   QFormLayout* ret {new QFormLayout};
   addTitle(ret,tr("Qt"));
   addQtObject(ret);
   addTitle(ret,tr("Basic Information"));
   addInheritance(ret);
   Base::addFields(ret);
   addTitle(ret,tr("Types"));
   addTypeButtons(ret);
   return ret;
}






bool Class::apply()
{
   if ( !_inheritanceEdit->text().isEmpty() && !_inheritanceEdit->hasAcceptableInput() )
   {
      QMessageBox box;
      box.setWindowTitle(tr("Invalid Inheritance String"));
      box.setText(tr("The inheritance string is not valid. Please change to valid string and try again."));
      box.setIcon(QMessageBox::Warning);
      box.addButton(QMessageBox::Ok);
      box.exec();
      return false;
   }
   if ( !Namespace::apply() ) return false;
   _block->setInheritance(_inheritanceEdit->text());
   _block->setQtObject(_qtObjectBox->isChecked());
   return true;
}






void Class::addInheritance(QFormLayout* layout)
{
   _inheritanceEdit = new QLineEdit;
   _inheritanceEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(CppQt::Class::_inheritanceRegExp)));
   _inheritanceEdit->setText(_block->inheritance());
   layout->addRow(new QLabel(tr("Inheritance:")),_inheritanceEdit);
}






void Class::addQtObject(QFormLayout* layout)
{
   _qtObjectBox = new QCheckBox(tr("Qt Object"));
   _qtObjectBox->setChecked(_block->isQtObject() || _block->hasSignalsOrSlots());
   _qtObjectBox->setDisabled(_block->hasSignalsOrSlots());
   layout->addRow(_qtObjectBox);
}
