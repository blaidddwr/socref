#include <memory>
#include <QDebug>
#include <QDir>
#include <exception.h>
#include "scanthread.h"
#include "abstractparserfactory.h"
#include "abstractparser.h"
#include "common.h"



using namespace std;






ScanThread::ScanThread(const AbstractParserFactory& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent):
   QThread(parent),
   _factory(factory)
{
   buildList(scanDirectory,filters);
}






int ScanThread::size() const
{
   return _list.size();
}






void ScanThread::run()
{
   try
   {
      for (int i = 0; i < _list.size() ;++i)
      {
         emit progressChanged(i);
         if ( isInterruptionRequested() )
         {
            return;
         }
         unique_ptr<AbstractParser> parser {_factory.makeParser(_list.at(i).completeBaseName(),_list.at(i).suffix())};
         if ( parser )
         {
            QFile file(_list.at(i).canonicalFilePath());
            if ( !file.open(QFile::ReadWrite|QIODevice::Text) )
            {
               Exception::SystemError e;
               MARK_EXCEPTION(e);
               e.setDetails(tr("Failed opening file %1: %2").arg(_list.at(i).canonicalFilePath()).arg(file.errorString()));
               throw e;
            }
            parser->execute(&file);
         }
      }
   }
   catch (Exception::Base e)
   {
      showException(tr("An error occured while scanning and parsing files."),e);
      return;
   }
   catch (std::exception e)
   {
      qDebug().nospace() << "Exception Caught!";
      qDebug().nospace() << e.what() << "\n";
      return;
   }
   catch (...)
   {
      qDebug() << "Exception Caught!\n";
      return;
   }
}






void ScanThread::buildList(const QString& scanDirectory, const QStringList& filters)
{
   QDir dir(scanDirectory);
   if ( !dir.exists() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Directory %1 does not exist.").arg(scanDirectory));
      throw e;
   }
   QFileInfoList list {dir.entryInfoList(filters)};
   for (auto item : list)
   {
      if ( item.isDir() ) buildList(item.canonicalFilePath(),filters);
      else _list << item;
   }
}
