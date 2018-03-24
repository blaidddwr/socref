#include "gui_scandialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include "scanthread.h"
#include "application.h"
#include "common.h"



using namespace Gui;






ScanDialog::ScanDialog(ScanThread* scanner, QWidget* parent):
   PersistentDialog("gui.scandialog.geometry",parent),
   _scanner(scanner)
{
   setupGui();
   connect(_scanner,&ScanThread::progressChanged,this,&ScanDialog::progressChanged);
   connect(_scanner,&ScanThread::finished,this,&ScanDialog::scanFinished);
}






int ScanDialog::exec()
{
   _scanner->start();
   return QDialog::exec();
}






void ScanDialog::closeEvent(QCloseEvent* event)
{
   _scanner->requestInterruption();
   event->ignore();
}






void ScanDialog::cancelClicked()
{
   _scanner->requestInterruption();
}






void ScanDialog::progressChanged(int complete)
{
   _bar->setValue(complete);
}






void ScanDialog::scanFinished()
{
   if ( _scanner->hasException() )
   {
      showException(tr("An error occured while scanning and parsing files."),_scanner->exception());
   }
   accept();
}






void ScanDialog::setupGui()
{
   setupBar();
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(_bar);
   layout->addLayout(setupBottom());
   setLayout(layout);
   setWindowTitle("Scanning Files");
}






void ScanDialog::setupBar()
{
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(_scanner->size());
}






QLayout* ScanDialog::setupBottom()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addStretch();
   ret->addWidget(setupCancel());
   ret->addStretch();
   return ret;
}






QWidget* ScanDialog::setupCancel()
{
   QPushButton* ret {new QPushButton(tr("&Cancel"))};
   connect(ret,&QPushButton::clicked,this,&ScanDialog::cancelClicked);
   return ret;
}
