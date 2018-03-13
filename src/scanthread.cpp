#include "scanthread.h"
#include <memory>
#include <QDebug>
#include <QDir>
#include "abstractparserfactory.h"
#include "abstractparser.h"



using namespace std;
//






/*!
 *
 * @param factory  
 *
 * @param scanDirectory  
 *
 * @param filters  
 *
 * @param parent  
 */
ScanThread::ScanThread(std::unique_ptr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent):
   QThread(parent),
   _factory(factory.get())
{
   factory.release()->setParent(this);
   buildList(scanDirectory,filters);
}






/*!
 */
int ScanThread::size() const
{
   return _list.size();
}






/*!
 */
bool ScanThread::hasException() const
{
   return _exception;
}






/*!
 */
const Exception::Base& ScanThread::exception() const
{
   if ( !_exception )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get exception when none has been set."));
      throw e;
   }
   return *_exception;
}






/*!
 */
void ScanThread::run()
{
   delete _exception;
   _exception = nullptr;
   try
   {
      for (int i = 0; i < _list.size() ;++i)
      {
         emit progressChanged(i);
         if ( isInterruptionRequested() )
         {
            return;
         }
         unique_ptr<AbstractParser> parser
         {
            _factory->make(_list.at(i).completeBaseName().toLower(),_list.at(i).suffix().toLower())
         };
         if ( parser )
         {
            QFile file(_list.at(i).canonicalFilePath());
            if ( !file.open(QFile::ReadWrite|QIODevice::Text) )
            {
               Exception::SystemError e;
               MARK_EXCEPTION(e);
               e.setDetails(
                        tr("Failed opening file %1: %2")
                        .arg(_list.at(i).canonicalFilePath())
                        .arg(file.errorString()));
               throw e;
            }
            parser->execute(&file);
         }
      }
   }
   catch (Exception::Base e)
   {
      _exception = new Exception::Base(e);
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






/*!
 *
 * @param scanDirectory  
 *
 * @param filters  
 */
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
