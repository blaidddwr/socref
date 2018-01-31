#include <QLineEdit>
#include <QRegExpValidator>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "cppqt_gui_typedialog.h"
#include "exception.h"



using namespace CppQt::Gui;



const char* TypeDialog::_typeRegExp {"\\s*(static\\s+)?(const(expr)?\\s+)?[a-zA-Z]+[a-zA-Z0-9_]*(<[a-zA-Z]+[a-zA-Z0-9_]*(,[a-zA-Z]+[a-zA-Z0-9_]*)*>)?(\\s*\\*(\\s*const)?)*\\s*&?\\s*"};






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
   if ( !_text->hasAcceptableInput() )
   {
      return QString();
   }
   return _text->text();
}






void TypeDialog::setName(const QString& name)
{
   if ( !QRegExp(_typeRegExp).exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set C++/Qt type name to invalid string '%1'").arg(name));
      throw e;
   }
   _text->setText(name);
}






void TypeDialog::textChanged(const QString& text)
{
   Q_UNUSED(text)
   _ok->setDisabled(!_text->hasAcceptableInput());
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
   _text->setValidator(new QRegExpValidator(QRegExp(_typeRegExp)));
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
