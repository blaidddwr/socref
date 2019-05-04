#include "scanthread.h"
#include <QDebug>
#include <QDir>
#include "exception.h"
#include "scanner.h"






/*!
 * Constructs a new scan thread with the given mapping of scanner objects,
 * directory to scan, file filters, and possible parent.
 *
 * @param scanMap The mapping of scanner objects this scan thread uses for
 *                parsing all source files it finds.
 *
 * @param scanDirectory The directory that this scan thread will scan for source
 *                      files.
 *
 * @param filters The file filters this scan thread will use for filtering the
 *                files in the given scan directory. Only files that match any
 *                of these filters will be scanned.
 *
 * @param parent The parent of this object if any.
 */
ScanThread::ScanThread(const QMap<QString,Scanner*>& scanMap, const QString& scanDirectory, const QStringList& filters, QObject* parent)
   :
   QThread(parent),
   _scanMap(scanMap),
   _scanDirectory(scanDirectory),
   _filters(filters)
{
   // Set the parent of all scanner objects in the given map to this object.
   for (auto scanner: _scanMap) scanner->setParent(this);
}






/*!
 * Implements _QThread_ interface. This iterates through all files to be parsed
 * and parses them, constantly checking for any requested interruption.
 */
void ScanThread::run()
{
   try
   {
      // Initialize the percent complete to zero and create the list of files to be
      // matched with scanners.
      int percentComplete {0};
      QFileInfoList list {createList(_scanDirectory,_filters)};

      // Iterate through the list of files to possibly be scanned.
      for (int i = 0; i < list.size() ;++i)
      {
         // Check to see if interruption of this thread is requested.
         if ( isInterruptionRequested() ) return;

         // Attempt to find a scanner object on this thread's mapping that matches the file
         // name.
         QFileInfo info {list.at(i)};
         auto j {_scanMap.find(info.fileName())};

         // If a scanner object was found then scan the current file with it.
         if ( j != _scanMap.end() ) (*j)->parse(info.absoluteFilePath());

         // Calculate percent complete and check to see if it has changed since last time.
         int newPercentComplete {i*100/list.size()};
         if ( newPercentComplete != percentComplete )
         {
            // Update the percent complete and emit the progress changed signal.
            percentComplete = newPercentComplete;
            emit progressChanged(percentComplete);
         }
      }
   }

   // Catch any exception thrown within this thread and emit it as a signal.
   catch (Exception e)
   {
      emit exceptionThrown(e);
   }
}






/*!
 * creates and returns a list of files that match the given given scan directory
 * and file filters. A file must match only one file filter to be added to the
 * list of matched files.
 *
 * @param scanDirectory The directory where files will be matched and added to
 *                      the returned list.
 *
 * @param filters The filters that are used to match files. Each string in this
 *                list must be a single filter.
 *
 * @return List of files that match the given scan directory and filters.
 */
QFileInfoList ScanThread::createList(const QString& scanDirectory, const QStringList& filters)
{
   // Initialize the return list.
   QFileInfoList ret;

   // Get the scan directory from the given path and make sure it exists.
   QDir dir(scanDirectory);
   if ( !dir.exists() )
   {
      throw Exception(tr("Scan directory %1 does not exist.").arg(scanDirectory));
   }

   // Make a list of files in the scan directory with the given filters and iterate
   // through them all.
   QFileInfoList list {dir.entryInfoList(filters)};
   for (auto item : list)
   {
      // If the file is a directory then recursively call this method on that directory
      // else add the file to this object's list of files to be parsed.
      if ( item.isDir() ) ret << createList(item.canonicalFilePath(),filters);
      else ret << item;
   }

   // Return the list of matched files.
   return ret;
}

