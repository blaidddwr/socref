#ifndef SCANTHREAD_H
#define SCANTHREAD_H
#include <memory>
#include <QThread>
#include <QFileInfoList>
#include <exception.h>
#include "global.h"



class ScanThread : public QThread
{
   Q_OBJECT
public:
   explicit ScanThread(std::unique_ptr<AbstractParserFactory>&& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent = nullptr);
   int size() const;
   bool hasException() const;
   const Exception::Base& exception() const;
signals:
   void progressChanged(int complete);
protected:
   virtual void run() override final;
private:
   void buildList(const QString& scanDirectory, const QStringList& filters);
   AbstractParserFactory* _factory;
   QFileInfoList _list;
   Exception::Base* _exception {nullptr};
};



#endif
