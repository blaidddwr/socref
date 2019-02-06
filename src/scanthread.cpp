#include "scanthread.h"
#include <QDebug>
#include <QDir>
#include "abstractparserfactory.h"
#include "abstractparser.h"



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
ScanThread::ScanThread(Soc::Ut::QPtr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent):
   QThread(parent),
   _factory(factory.get())
{
   // Set this object as the given factory's parent and build the list of files to 
   // parse. 
   factory.release(this);
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
 * Implements _QThread_ interface. This iterates through all files to be parsed and 
 * parses them, constantly checking for any requested interruption. 
 */
void ScanThread::run()
{
   // Iterate through this object's list of files to be parsed. 
   for (int i = 0; i < _list.size() ;++i)
   {
      // Check to see if interruption of this thread is requested. 
      if ( isInterruptionRequested() ) return;

      // Signal progress has occurred. 
      emit progressChanged(i);

      // Make a parser object for the current file using this object's parser factory 
      // and run it if it is not null. 
      const QFileInfo info {_list.at(i)};
      Soc::Ut::QPtr<AbstractParser> parser
      {
         _factory->make(info.completeBaseName().toLower(),info.suffix().toLower())
      };
      if ( parser ) parse(parser.get(),info);
   }
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
   // Open the given file and make sure it did not fail. 
   QFile file(info.canonicalFilePath());
   bool ok {file.open(QFile::ReadWrite|QIODevice::Text)};
   Q_ASSERT(ok);

   // parse the opened file with the given parser. 
   parser->execute(&file);
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
   // Get the scan directory from the given path and make sure it exists. 
   QDir dir(scanDirectory);
   Q_ASSERT(dir.exists());

   // Make a list of files in the scan directory with the given filters and iterate 
   // through them all. 
   QFileInfoList list {dir.entryInfoList(filters)};
   for (auto item : list)
   {
      // If the file is a directory then recursively call this method on that directory 
      // else add the file to this object's list of files to be parsed. 
      if ( item.isDir() ) buildList(item.canonicalFilePath(),filters);
      else _list << item;
   }
}
