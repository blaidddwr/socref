#include <QLineEdit>
#include <QRegExpValidator>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <exception.h>
#include "cppqt_gui_typedialog.h"
#include "cppqt_common.h"
#include "application.h"



using namespace CppQt::Gui;
const char* TypeDialog::_geometryKey {"cppqt.gui.typedialog.geometry"};






TypeDialog::TypeDialog(QWidget* parent):
   QDialog(parent)
{
   setupGui();
   restoreSettings();
}






TypeDialog::TypeDialog(const QString& name, QWidget* parent):
   QDialog(parent)
{
   setupGui();
   restoreSettings();
   setName(name);
}






TypeDialog::~TypeDialog()
{
   saveSettings();
}






QString TypeDialog::name()
{
   if ( !_text->hasAcceptableInput() ) return QString();
   return _text->text();
}






void TypeDialog::setName(const QString& name)
{
   if ( !TypeDialog::isValidTypeString(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set C++/Qt type name to invalid string '%1'").arg(name));
      throw e;
   }
   _text->setText(name);
}






bool TypeDialog::isValidTypeString(const QString& input)
{
   bool ret {false};
   if ( QRegExp("\\s*(const\\s+)?((::)?[a-zA-Z_]+[a-z-A-Z0-9_]*)+(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?\\s*").exactMatch(input) )
   {
      ret = true;
      if ( input.contains('<') )
      {
         int begin {input.indexOf('<')};
         int end {input.lastIndexOf('>')};
         if ( !isValidTemplateArgument(input.mid(begin+1,end-begin-1)) ) ret = false;
      }
   }
   return ret;
}






void TypeDialog::textChanged(const QString& text)
{
   Q_UNUSED(text)
   _ok->setDisabled(!isValidTypeString(text));
}






void TypeDialog::restoreSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}






void TypeDialog::saveSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
}






void TypeDialog::setupGui()
{
   setupLineEdit();
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_text);
   layout->addLayout(setupButtons());
   setLayout(layout);
}






void TypeDialog::setupLineEdit()
{
   _text = new QLineEdit;
   connect(_text,&QLineEdit::textChanged,this,&TypeDialog::textChanged);
}






QLayout* TypeDialog::setupButtons()
{
   _ok = new QPushButton(tr("Ok"));
   _ok->setDisabled(true);
   QPushButton* cancel {new QPushButton(tr("Cancel"))};
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(_ok);
   ret->addStretch();
   ret->addWidget(cancel);
   connect(_ok,&QPushButton::clicked,this,&QDialog::accept);
   connect(cancel,&QPushButton::clicked,this,&QDialog::reject);
   return ret;
}
