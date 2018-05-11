#ifndef SCANTHREAD_H
#define SCANTHREAD_H
#include <memory>
#include <QThread>
#include <QFileInfoList>
#include <exception.h>
#include "global.h"
//



/*!
 * This implements the qt thread class and is used to scan and parse an entire 
 * directory of source files with the given file filters and parser factory. This 
 * executes the scanning of files on the separate thread started by the qt thread 
 * class. Signals are emitted for the progress of the scanning and when it is 
 * complete. Because execution of parsing is done on a separate thread if an 
 * exception is thrown within that thread it is caught and saved within the class. 
 * This exception can be viewed from the main thread. 
 * 
 * two 
 * 
 * three 
 */
class ScanThread : public QThread
{
   Q_OBJECT
public:
   explicit ScanThread(std::unique_ptr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent = nullptr);
   int size() const;
   bool hasException() const;
   const Exception::Base& exception() const;
signals:
   /*!
    * Signals that the scan thread has started working on the given file index. The 
    * indexing starts at zero and ends at one less than the total number of files to 
    * process. 
    *
    * @param index The current file index that is being parsed. 
    */
   void progressChanged(int index);
protected:
   virtual void run() override final;
private:
   void buildList(const QString& scanDirectory, const QStringList& filters);
   bool run(const QFileInfo& info);
   static void parse(AbstractParser* parser, const QFileInfo& info);
   /*!
    * A pointer to the parser factory this scan thread uses for making parser objects. 
    * 
    * two 
    * 
    * three 
    */
   AbstractParserFactory* _factory;
   /*!
    * A list of matched files that his scan thread parses. 
    */
   QFileInfoList _list;
   /*!
    * A pointer that this scan thread uses to save any Socrates exception that occurs 
    * within its separate scanning thread. 
    */
   Exception::Base* _exception {nullptr};
};



#endif
