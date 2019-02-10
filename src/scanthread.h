#ifndef SCANTHREAD_H
#define SCANTHREAD_H
#include <QThread>
#include <QMap>
#include <QFileInfoList>
#include "global.h"
//



/*!
 * This implements the qt thread class and is used to scan and parse an entire 
 * directory of source files with the given file filters and mapping of scanner 
 * objects. This executes the scanning of files on the separate thread started by 
 * the qt thread class. Signals are emitted for the progress of the scanning and if 
 * an exception is thrown from within the thread. 
 * 
 * Because execution of parsing is done on a separate thread if an exception is 
 * thrown within that thread it is caught and emitted as a signal that can be used 
 * on the main GUI thread. 
 * 
 * The mapping of scanner objects are used to determine which scanner object to use 
 * on any matched files found. The key of the mapping is used as the file name to 
 * be matched. This thread takes ownership of all scanner objects passed to it in 
 * the given map and is destroyed when this thread object is destroyed. 
 */
class ScanThread : public QThread
{
   Q_OBJECT
public:
   explicit ScanThread(const QMap<QString,Scanner*>& scanMap, const QString& scanDirectory, const QStringList& filters, QObject* parent = nullptr);
   virtual ~ScanThread() override final;
signals:
   /*!
    * Signals that the progress of this scan thread has changed to the percentage 
    * given. 
    *
    * @param index The percent complete of this scan thread out of one hundred. 
    */
   void progressChanged(int index);
   /*!
    * Signals that an exception was thrown and caught in this thread, prematurely 
    * ending the scanning of source files in this thread. 
    *
    * @param e The exception that was thrown and caught in this thread. 
    */
   void exceptionThrown(const Exception& e);
protected:
   virtual void run() override final;
private:
   static QFileInfoList createList(const QString& scanDirectory, const QStringList& filters);
private:
   /*!
    * The mapping of scanner objects this scan thread uses for parsing all source 
    * files it finds. 
    */
   const QMap<QString,Scanner*> _scanMap;
   /*!
    * The directory that this scan thread will scan for source files. 
    */
   const QString _scanDirectory;
   /*!
    * The file filters this scan thread will use for filtering the files in the given 
    * scan directory. Only files that match any of these filters will be scanned. 
    */
   const QStringList _filters;
};



#endif
