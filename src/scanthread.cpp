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
 */
ScanThread::ScanThread(std::unique_ptr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent):
   QThread(parent),
   _factory(factory.get())
{
   // Set the given parser factory's parent to this scan thread and build a list of 
   // files that will be parsed in the given scan directory. 
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
 */
const Exception::Base& ScanThread::exception() const
{
   // If this scan thread has no saved exception then throw an exception, else return 
   // a reference to the exception that occurred in this scan thread. 
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
 * of that thread begins by calling the qt execute function. This implementation 
 * parses of all matched files from the given scan directory. 
 */
void ScanThread::run()
{
   // Delete this scan thread's saved exception if any and set the pointer to null. 
   delete _exception;
   _exception = nullptr;

   // Iterate through this object's saved list of files to be parsed, emitting the 
   // progress changed signal for each index and parsing each file. 
   try
   {
      for (int i = 0; i < _list.size() ;++i)
      {
         emit progressChanged(i);
         if ( !run(_list.at(i)) ) return;
      }
   }

   // If any exception occurs while running this function then catch it and exit from 
   // this function immediately. If the exception is a Socrates one then save it to 
   // this scan thread's exception pointer else just report it to qt debug. 
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
 */
void ScanThread::buildList(const QString& scanDirectory, const QStringList& filters)
{
   // If the given directory does not exist then throw an exception. 
   QDir dir(scanDirectory);
   if ( !dir.exists() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Directory %1 does not exist.").arg(scanDirectory));
      throw e;
   }

   // Get a directory listing of all files and directories within it that match the 
   // given filters. Iterate through the list and add all matches that are not 
   // directories to this scan thread's list of matched files. 
   QFileInfoList list {dir.entryInfoList(filters)};
   for (auto item : list)
   {
      if ( item.isDir() ) buildList(item.canonicalFilePath(),filters);
      else _list << item;
   }
}






/*!
 * Attempts to parse the given file is this object's parser factory makes a valid 
 * parser. This also checks if interruption is requested returning false if it is 
 * requested. 
 *
 * @param info Info for the file that may be parsed. 
 *
 * @return False if interruption of this thread is requested or true otherwise. 
 */
bool ScanThread::run(const QFileInfo& info)
{
   // If interruption is requested then return false. 
   if ( isInterruptionRequested() )
   {
      return false;
   }

   // Make a new parser from this object's parser factory with the given file's 
   // information, saving it to the smart pointer _parser_. If _parser_ is not null 
   // then use it to parse the given file. 
   unique_ptr<AbstractParser> parser
   {
      _factory->make(info.completeBaseName().toLower(),info.suffix().toLower())
   };
   if ( parser )
   {
      parse(parser.get(),info);
   }

   // Return true signaling interruption is not requested and parsing should 
   // continue. 
   return true;
}






/*!
 * Parses the given file using the given parser. If the file fails to open an 
 * exception is thrown. 
 *
 * @param parser Pointer to the abstract parser that is used for parsing the given 
 *               file. 
 *
 * @param info Info for the file that is parsed with the given parser. 
 */
void ScanThread::parse(AbstractParser* parser, const QFileInfo& info)
{
   // Open the given file for reading and writing and interpreted as a text using the 
   // qt file device _file_. If opening the file fails then throw an exception. 
   QFile file(info.canonicalFilePath());
   if ( !file.open(QFile::ReadWrite|QIODevice::Text) )
   {
      Exception::SystemError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed opening file %1: %2")
                   .arg(info.canonicalFilePath())
                   .arg(file.errorString()));
      throw e;
   }

   // Call the execute method of the given parser with the qt file _file_. 
   parser->execute(&file);
}
