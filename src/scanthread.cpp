#include "scanthread.h"
#include <memory>
#include <QDebug>
#include <QDir>
#include "abstractparserfactory.h"
#include "abstractparser.h"



using namespace std;
//






/*!
 * This constructs a new scan thread with the given parser factory, directory to 
 * scan, file filters, and possible parent. 
 *
 * @param factory The parser factory that this scan thread will use to make all 
 *                parser objects for each matched source file. This scan thread 
 *                takes ownership of the factory. 
 *
 * @param scanDirectory The directory that this scan thread will scan for source 
 *                      files. 
 *
 * @param filters The file filters this scan thread will use for filtering the 
 *                files in the given scan directory. Only files that match any of 
 *                these filters will be parsed. 
 *
 * @param parent The parent of this object if any. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set the given parser factory's parent to this scan thread and build a list of 
 *    files that will be parsed in the given scan directory. 
 */
ScanThread::ScanThread(std::unique_ptr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent):
   QThread(parent),
   _factory(factory.get())
{
   factory.release()->setParent(this);
   buildList(scanDirectory,filters);
}






/*!
 * Returns the total number of files that were matched for parsing. 
 *
 * @return Total number of files matched for parsing. 
 */
int ScanThread::size() const
{
   return _list.size();
}






/*!
 * Tests if an exception occurred in this scan thread's separate thread while 
 * parsing source files. 
 *
 * @return True if an exception occurred else false it scanning was successful with 
 *         no error. 
 */
bool ScanThread::hasException() const
{
   return _exception;
}






/*!
 * Returns a reference to the exception that occurred in this scan thread's 
 * separate thread causing failure. If this scan thread has no exception this 
 * itself will throw an exception. 
 *
 * @return Reference to exception that occurred while parsing source files. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this scan thread has no saved exception then throw an exception, else 
 *    return a reference to the exception that occurred in this scan thread. 
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
 * This implements the interface that is called within the Qt thread when execution 
 * of that thread begins by calling the qt execute function. This scan thread using 
 * this function to implement the parsing of all matched files from the given scan 
 * directory. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete this scan thread's saved exception if any and set the pointer to null. 
 *    For the proceeding steps iterate through all the matched source files 
 *    starting at index 0. 
 *
 * 2. Signal the index of the source file that is being worked on. If interruption 
 *    of this thread is requested return and exit this function immediately, else 
 *    go to the next step. 
 *
 * 3. Create a new parser object from the given parser factory. If the returned 
 *    parser object is not null then run its execution method to parse the source 
 *    file. 
 *
 * 4. If any exception occurs while running this function then catch it and exit 
 *    from this function immediately. If the exception is a Socrates one then save 
 *    it to this scan thread's exception pointer. 
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
               e.setDetails(tr("Failed opening file %1: %2")
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
 * Builds this scan thread's list of matched files that will be parsed from the 
 * given scan directory and file filters. A file must match only one file filter to 
 * be added to the list of matched files. 
 *
 * @param scanDirectory The directory where files will be matched and added to the 
 *                      list. 
 *
 * @param filters The filters that will be used to match files. Each string in this 
 *                list must be a single filter. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the directory does not exist then throw an exception, else go to the next 
 *    step. 
 *
 * 2. Get a directory listing of all files and directories within it that match the 
 *    given filters. Iterate through the list and add all matches that are not 
 *    directories to this scan thread's list of matched files. 
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
