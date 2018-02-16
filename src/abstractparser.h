#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H
#include <QObject>



class QFile;



class AbstractParser : public QObject
{
   Q_OBJECT
public:
   explicit AbstractParser() = default;
   explicit AbstractParser(AbstractParser* parent);
   virtual ~AbstractParser();
   void execute(QFile* file);
protected:
   virtual AbstractParser* readLine(const QString& line) = 0;
   virtual void makeOutput() = 0;
   void addLine(const QString& line = QString(), int indent = 0);
private:
   void read(QFile* file);
   void processInput();
   void processOutput();
   void write(QFile* file);
   QString _origional;
   QStringList* _input {nullptr};
   int* _index {nullptr};
   QStringList* _output {nullptr};
   AbstractParser* _root {nullptr};
   QList<AbstractParser*> _children;
};



#endif
