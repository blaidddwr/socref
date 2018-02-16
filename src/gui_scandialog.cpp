#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QCloseEvent>
#include "gui_scandialog.h"
#include "scanthread.h"
#include "application.h"



using namespace Gui;
const char* ScanDialog::_geometryKey {"gui.scandialog.geometry"};






ScanDialog::ScanDialog(ScanThread* scanner, QWidget* parent):
   QDialog(parent),
   _scanner(scanner)
{
   setupGui();
   restoreSettings();
   connect(_scanner,&ScanThread::progressChanged,this,&ScanDialog::progressChanged);
   connect(_scanner,&ScanThread::finished,this,&ScanDialog::scanFinished);
}






ScanDialog::~ScanDialog()
{
   saveSettings();
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
   accept();
}






void ScanDialog::restoreSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}






void ScanDialog::saveSettings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
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
