#include <QLineEdit>
#include <QTableWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include "cppqt_type_gui_edit.h"
#include "cppqt_abstracttype.h"
#include "cppqt_type_concrete.h"
#include "cppqt_type_template.h"



using namespace std;
using namespace CppQt;
using namespace CppQt::Type::Gui;






Edit::Edit(AbstractType* type, QWidget* parent):
   QWidget(parent)
{
   createNameEdit();
   createSpinBox();
   createTable();
   QFormLayout* formLayout {new QFormLayout};
   formLayout->addRow(new QLabel(tr("Name:")),_nameEdit);
   formLayout->addRow(new QLabel(tr("Size:")),_spinBox);
   formLayout->addRow(new QLabel(tr("Variants:")),_variantTable);
   createGroupBox(formLayout);
   QHBoxLayout* layout{new QHBoxLayout};
   layout->addWidget(_groupBox);
   setLayout(layout);
   setType(type);
}






Edit& Edit::setType(AbstractType* type)
{
   _nameEdit->clear();
   _spinBox->setValue(0);
   if ( type )
   {
      _nameEdit->setText(type->name());
      if ( Template* type_ = dynamic_cast<Template*>(type) )
      {
         _spinBox->setValue(type_->variants().size());
         int row {0};
         for (auto name : type_->variants())
         {
            _variantTable->setItem(row++,0,new QTableWidgetItem(name));
         }
      }
   }
   return *this;
}






unique_ptr<AbstractType> Edit::type() const
{
   if ( !isValid() )
   {
      return nullptr;
   }
   unique_ptr<AbstractType> ret;
   if ( _variantTable->rowCount() > 0 )
   {
      Template* type;
      ret = unique_ptr<AbstractType>(type = new Type::Template);
      QList<QString> variants;
      for (int i = 0; i < _variantTable->rowCount() ;++i)
      {
         variants.append(_variantTable->item(i,0)->text());
      }
      type->setVariants(variants);
   }
   else
   {
      ret = unique_ptr<AbstractType>(new Type::Concrete);
   }
   ret->setName(_nameEdit->text());
   return ret;
}






Edit& Edit::setTitle(const QString& title)
{
   _groupBox->setTitle(title);
   return *this;
}






bool Edit::isValid(bool informUser) const
{
   if ( !_nameEdit->hasAcceptableInput() )
   {
      if ( informUser )
      {
         QMessageBox box;
         box.setWindowTitle(tr("Invalid base name."));
         box.setText(tr("'%1' is not a valid base type name.").arg(_nameEdit->text()));
         box.setIcon(QMessageBox::Warning);
         box.exec();
      }
      return false;
   }
   QRegExp expr("[a-zA-Z_]+[a-zA-Z0-9_]*");
   for (int i = 0; i < _variantTable->rowCount() ;++i)
   {
      if ( !_variantTable->item(i,0) )
      {
         if ( informUser )
         {
            QMessageBox box;
            box.setWindowTitle(tr("Invalid variant name."));
            box.setText(tr("'' is not a valid variant name."));
            box.setIcon(QMessageBox::Warning);
            box.exec();
         }
         return false;
      }
      if ( !expr.exactMatch(_variantTable->item(i,0)->text()) )
      {
         if ( informUser )
         {
            QMessageBox box;
            box.setWindowTitle(tr("Invalid variant name."));
            box.setText(tr("'%1' is not a valid variant name.").arg(_variantTable->item(i,0)->text()));
            box.setIcon(QMessageBox::Warning);
            box.exec();
         }
         return false;
      }
   }
   return true;
}






void Edit::sizeValueChanged(int i)
{
   _variantTable->setRowCount(i);
}






void Edit::createNameEdit()
{
   _nameEdit = new QLineEdit;
   _nameEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*"),this));
}






void Edit::createSpinBox()
{
   _spinBox = new QSpinBox;
   _spinBox->setMinimum(0);
   _spinBox->setMaximum(INT_MAX);
   _spinBox->setValue(0);
   connect(_spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&Edit::sizeValueChanged);
}






void Edit::createTable()
{
   _variantTable = new QTableWidget;
   _variantTable->setColumnCount(1);
   _variantTable->setHorizontalHeaderLabels(QStringList() << "Name");
}






void Edit::createGroupBox(QLayout* layout)
{
   _groupBox = new QGroupBox;
   _groupBox->setLayout(layout);
}
