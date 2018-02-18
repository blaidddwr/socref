#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H
#include <QObject>



class QFile;



class AbstractParser : public QObject
{
   Q_OBJECT
public:
   explicit AbstractParser();
   explicit AbstractParser(AbstractParser* parent);
   virtual ~AbstractParser();
   void execute(QFile* file);
protected:
   virtual bool readLine(const QString& line) = 0;
   virtual void makeOutput() = 0;
   void stepIntoChild(AbstractParser* child);
   int indent();
   void setIndent(int indent);
   void addLine(const QString& line);
   void addBlankLines(int count);
private:
   void read(QFile* file);
   void processInput();
   void processOutput();
   void write(QFile* file);
   QString _origional;
   QStringList* _input {nullptr};
   int* _index {nullptr};
   int* _indent {nullptr};
   QStringList* _output {nullptr};
   AbstractParser* _root {nullptr};
   QList<AbstractParser*> _children;
   AbstractParser* _child {nullptr};
};



#endif
