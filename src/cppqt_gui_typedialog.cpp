#include <QLineEdit>
#include <QRegExpValidator>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "cppqt_gui_typedialog.h"
#include "exception.h"



using namespace CppQt::Gui;






TypeDialog::TypeDialog(QWidget* parent):
   QDialog(parent)
{
   createGui();
}






TypeDialog::TypeDialog(const QString& name, QWidget* parent):
   QDialog(parent)
{
   createGui();
   setName(name);
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
   if ( QRegExp("\\s*(const\\s+)?[a-zA-Z_]+[a-z-A-Z0-9_]*(<(.*)>)?(\\s*\\*(\\s*const)?)*\\s*&?\\s*").exactMatch(input) )
   {
      ret = true;
      if ( input.contains('<') )
      {
         int begin {input.indexOf('<')};
         int end {input.lastIndexOf('>')};
         QStringList args {input.mid(begin+1,end-begin-1).split(',')};
         for (auto arg : args)
         {
            ret &= isValidTypeString(arg);
         }
      }
   }
   return ret;
}






void TypeDialog::textChanged(const QString& text)
{
   Q_UNUSED(text)
   _ok->setDisabled(!isValidTypeString(text));
}






void TypeDialog::createGui()
{
   createLineEdit();
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_text);
   layout->addLayout(createButtons());
   setLayout(layout);
}






void TypeDialog::createLineEdit()
{
   _text = new QLineEdit;
   connect(_text,&QLineEdit::textChanged,this,&TypeDialog::textChanged);
}






QLayout* TypeDialog::createButtons()
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
