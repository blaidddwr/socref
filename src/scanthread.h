#ifndef SCANTHREAD_H
#define SCANTHREAD_H
#include <QThread>
#include <QFileInfoList>
#include "global.h"



class ScanThread : public QThread
{
   Q_OBJECT
public:
   explicit ScanThread(const AbstractParserFactory& factory, const QString& scanDirectory, const QStringList& filters, QObject* parent = nullptr);
   int size() const;
signals:
   void progressChanged(int complete);
protected:
   virtual void run() override final;
private:
   void buildList(const QString& scanDirectory, const QStringList& filters);
   const AbstractParserFactory& _factory;
   QFileInfoList _list;
};



#endif
